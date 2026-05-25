# SafeCracker Web

Python/Flask webhook and leaderboard for ESP32 scores.

## Run locally

```sh
python -m venv .venv
. .venv/bin/activate
pip install -r requirements.txt
python app.py
```

The app listens on `http://localhost:8000` when run directly.

## Run on server with Podman

```sh
podman compose -f webpage/compose.yaml up -d --build
```

Caddy can proxy Cloudflare traffic to:

```caddyfile
ruc-esp.nerizz.com {
  reverse_proxy 127.0.0.1:18080
}
```

The compose file maps host port `18080` to container port `8000`, so it will not conflict with another host service already using `8000`.

If Caddy also runs as a container in the same Podman network, remove the `ports` block and proxy directly to the service name:

```caddyfile
ruc-esp.nerizz.com {
  reverse_proxy safecracker-web:8000
}
```

ESP32 posts scores to:

```text
POST https://ruc-esp.nerizz.com/api/scores
```

`POST /` also accepts the same payload, so a device configured with only `https://ruc-esp.nerizz.com` will still upload scores.

Example payload:

```json
{
  "device_id": "SC-1234",
  "level": 1,
  "level_name": "Level 1",
  "time_ms": 18500,
  "stability": 92,
  "targets_hit": 5,
  "target_count": 5
}
```
