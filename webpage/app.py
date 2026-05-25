import json
import os
import sqlite3
import uuid
from datetime import datetime, timezone

from flask import Flask, g, jsonify, redirect, render_template, request, url_for


DATABASE_PATH = os.environ.get("SAFECRACKER_DB", "safecracker.sqlite3")

app = Flask(__name__)


def get_db():
    if "db" not in g:
        database_dir = os.path.dirname(DATABASE_PATH)
        if database_dir:
            os.makedirs(database_dir, exist_ok=True)
        g.db = sqlite3.connect(DATABASE_PATH)
        g.db.row_factory = sqlite3.Row
    return g.db


@app.teardown_appcontext
def close_db(error=None):
    db = g.pop("db", None)
    if db is not None:
        db.close()


def init_db():
    db = get_db()
    db.execute(
        """
        CREATE TABLE IF NOT EXISTS scores (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          device_id TEXT NOT NULL,
          level INTEGER NOT NULL,
          level_name TEXT NOT NULL,
          time_ms INTEGER NOT NULL,
          stability INTEGER NOT NULL,
          targets_hit INTEGER NOT NULL,
          target_count INTEGER NOT NULL,
          player_name TEXT,
          claim_token TEXT NOT NULL,
          created_at TEXT NOT NULL,
          targets_json TEXT,
          target_stats_json TEXT,
          replay_json TEXT
        )
        """
    )
    existing_columns = {
        row["name"]
        for row in db.execute("PRAGMA table_info(scores)").fetchall()
    }
    for column_name in ("targets_json", "target_stats_json", "replay_json"):
        if column_name not in existing_columns:
            db.execute(f"ALTER TABLE scores ADD COLUMN {column_name} TEXT")
    db.commit()


@app.before_request
def ensure_db():
    init_db()


def row_to_dict(row):
    return {
        "id": row["id"],
        "device_id": row["device_id"],
        "level": row["level"],
        "level_name": row["level_name"],
        "time_ms": row["time_ms"],
        "time_seconds": round(row["time_ms"] / 1000, 2),
        "stability": row["stability"],
        "targets_hit": row["targets_hit"],
        "target_count": row["target_count"],
        "player_name": row["player_name"],
        "created_at": row["created_at"],
        "has_details": bool(row["target_stats_json"] or row["replay_json"]),
    }


def clamp_int(value, minimum, maximum, field_name):
    try:
        parsed = int(value)
    except (TypeError, ValueError):
        raise ValueError(f"{field_name} must be an integer")
    return max(minimum, min(maximum, parsed))


def parse_json_list(value, field_name, max_items):
    if value is None or value == "":
        return []

    if isinstance(value, str):
        try:
            value = json.loads(value)
        except json.JSONDecodeError:
            raise ValueError(f"{field_name} must be valid JSON")

    if not isinstance(value, list):
        raise ValueError(f"{field_name} must be a list")

    return value[:max_items]


def load_json_list(row, column_name):
    value = row[column_name]
    if not value:
        return []

    try:
        parsed = json.loads(value)
    except json.JSONDecodeError:
        return []

    return parsed if isinstance(parsed, list) else []


@app.get("/")
def index():
    db = get_db()
    leaderboard = db.execute(
        """
        SELECT *
        FROM scores
        WHERE player_name IS NOT NULL
        ORDER BY level ASC, time_ms ASC, stability DESC, created_at ASC
        LIMIT 100
        """
    ).fetchall()
    pending = db.execute(
        """
        SELECT *
        FROM scores
        WHERE player_name IS NULL
        ORDER BY created_at DESC
        LIMIT 8
        """
    ).fetchall()
    recent = db.execute(
        """
        SELECT *
        FROM scores
        ORDER BY created_at DESC
        LIMIT 40
        """
    ).fetchall()
    stats = db.execute(
        """
        SELECT
          COUNT(*) AS total_scores,
          ROUND(AVG(time_ms)) AS avg_time_ms,
          ROUND(AVG(stability)) AS avg_stability,
          MAX(stability) AS best_stability
        FROM scores
        """
    ).fetchone()
    by_level = db.execute(
        """
        SELECT
          level,
          level_name,
          COUNT(*) AS attempts,
          ROUND(AVG(time_ms)) AS avg_time_ms,
          ROUND(AVG(stability)) AS avg_stability
        FROM scores
        GROUP BY level, level_name
        ORDER BY level ASC
        """
    ).fetchall()
    return render_template(
        "index.html",
        leaderboard=[row_to_dict(row) for row in leaderboard],
        pending=[row_to_dict(row) for row in pending],
        recent=[row_to_dict(row) for row in reversed(recent)],
        stats=dict(stats),
        by_level=[dict(row) for row in by_level],
    )


@app.post("/")
@app.post("/api/scores")
def create_score():
    payload = request.get_json(silent=True) or request.form
    try:
        device_id = str(payload.get("device_id", "unknown"))[:80]
        level = clamp_int(payload.get("level"), 1, 999, "level")
        level_name = str(payload.get("level_name") or f"Level {level}")[:80]
        time_ms = clamp_int(payload.get("time_ms"), 0, 3_600_000, "time_ms")
        stability = clamp_int(payload.get("stability"), 0, 100, "stability")
        targets_hit = clamp_int(payload.get("targets_hit", 0), 0, 999, "targets_hit")
        target_count = clamp_int(payload.get("target_count", targets_hit), 0, 999, "target_count")
        targets = parse_json_list(payload.get("targets"), "targets", 32)
        target_stats = parse_json_list(payload.get("target_stats"), "target_stats", 32)
        replay = parse_json_list(payload.get("replay"), "replay", 500)
    except ValueError as error:
        return jsonify({"error": str(error)}), 400

    claim_token = uuid.uuid4().hex
    created_at = datetime.now(timezone.utc).isoformat()
    db = get_db()
    cursor = db.execute(
        """
        INSERT INTO scores (
          device_id, level, level_name, time_ms, stability,
          targets_hit, target_count, claim_token, created_at,
          targets_json, target_stats_json, replay_json
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        """,
        (
            device_id,
            level,
            level_name,
            time_ms,
            stability,
            targets_hit,
            target_count,
            claim_token,
            created_at,
            json.dumps(targets, separators=(",", ":")),
            json.dumps(target_stats, separators=(",", ":")),
            json.dumps(replay, separators=(",", ":")),
        ),
    )
    db.commit()

    score_id = cursor.lastrowid
    return (
        jsonify(
            {
                "ok": True,
                "id": score_id,
                "claim_url": url_for("index", _external=True),
            }
        ),
        201,
    )


@app.get("/api/scores")
def list_scores():
    rows = get_db().execute("SELECT * FROM scores ORDER BY created_at DESC LIMIT 100").fetchall()
    return jsonify([row_to_dict(row) for row in rows])


@app.get("/api/scores/<int:score_id>")
def get_score(score_id):
    row = get_db().execute("SELECT * FROM scores WHERE id = ?", (score_id,)).fetchone()
    if row is None:
        return jsonify({"error": "score not found"}), 404

    score = row_to_dict(row)
    score["targets"] = load_json_list(row, "targets_json")
    score["target_stats"] = load_json_list(row, "target_stats_json")
    score["replay"] = load_json_list(row, "replay_json")
    return jsonify(score)


@app.get("/scores/<int:score_id>")
def score_detail(score_id):
    row = get_db().execute("SELECT * FROM scores WHERE id = ?", (score_id,)).fetchone()
    if row is None:
        return redirect(url_for("index"))

    score = row_to_dict(row)
    targets = load_json_list(row, "targets_json")
    target_stats = load_json_list(row, "target_stats_json")
    replay = load_json_list(row, "replay_json")
    return render_template(
        "score_detail.html",
        score=score,
        targets=targets,
        target_stats=target_stats,
        replay=replay,
    )


@app.post("/scores/<int:score_id>/claim")
def claim_score(score_id):
    player_name = request.form.get("player_name", "").strip()[:40]
    token = request.form.get("token", "").strip()

    if not player_name:
        return redirect(url_for("index"))

    row = get_db().execute("SELECT claim_token FROM scores WHERE id = ?", (score_id,)).fetchone()
    if row is None:
        return redirect(url_for("index"))

    if token and token != row["claim_token"]:
        return redirect(url_for("index"))

    get_db().execute("UPDATE scores SET player_name = ? WHERE id = ?", (player_name, score_id))
    get_db().commit()
    return redirect(url_for("index"))


@app.get("/healthz")
def healthz():
    return jsonify({"ok": True})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=int(os.environ.get("PORT", "8000")))
