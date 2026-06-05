/**
 * @file replay.js
 * @brief Interaktivni replay kretanja trake za jedan SafeCracker score.
 */

const replayStage = document.getElementById("replayStage");
const toggle = document.getElementById("replayToggle");
const scrub = document.getElementById("replayScrub");
const timeLabel = document.getElementById("replayTime");
const replay = window.safeCrackerReplay || [];
const targets = window.safeCrackerTargets || [];

let playing = false;
let startedAt = 0;
let pausedAt = 0;
let animationFrame = 0;
const durationMs = replay.length ? replay[replay.length - 1].t : 0;
const elements = {};

/**
 * @brief Pretvara fill vrijednost od -1.0 do 1.0 u CSS postotak.
 * @param {number|string} fill Popunjenost trake.
 * @returns {string} CSS postotak od 0% do 100%.
 */
function fillToPercent(fill) {
  return `${Math.max(0, Math.min(100, ((Number(fill) + 1) / 2) * 100))}%`;
}

/**
 * @brief Normalizira naziv replay dogadjaja za prikaz.
 * @param {string} eventName Naziv dogadjaja iz API payload-a.
 * @returns {string} Kratka oznaka dogadjaja.
 */
function eventLabel(eventName) {
  if (eventName === "target_hit") return "target hit";
  return eventName || "move";
}

/**
 * @brief Gradi DOM elemente replay trake, markera i targeta.
 */
function buildReplayStage() {
  if (!replayStage) return;

  replayStage.textContent = "";

  const axis = document.createElement("div");
  axis.className = "replay-axis";

  const leftLabel = document.createElement("span");
  leftLabel.textContent = "left";
  const centerLabel = document.createElement("span");
  centerLabel.textContent = "center";
  const rightLabel = document.createElement("span");
  rightLabel.textContent = "right";

  const track = document.createElement("div");
  track.className = "replay-track";

  const activeTarget = document.createElement("div");
  activeTarget.className = "replay-active-target";

  const marker = document.createElement("div");
  marker.className = "replay-marker";

  const readout = document.createElement("div");
  readout.className = "replay-readout";

  const targetLayer = document.createElement("div");
  targetLayer.className = "replay-target-layer";

  targets.forEach((target, index) => {
    const targetNode = document.createElement("div");
    targetNode.className = "replay-target";
    targetNode.style.left = fillToPercent(target);
    targetNode.dataset.index = String(index);
    targetNode.textContent = `#${index + 1}`;
    targetLayer.appendChild(targetNode);
  });

  track.appendChild(activeTarget);
  track.appendChild(marker);
  track.appendChild(targetLayer);
  axis.appendChild(leftLabel);
  axis.appendChild(centerLabel);
  axis.appendChild(rightLabel);
  replayStage.appendChild(readout);
  replayStage.appendChild(track);
  replayStage.appendChild(axis);

  elements.activeTarget = activeTarget;
  elements.marker = marker;
  elements.readout = readout;
  elements.targets = Array.from(targetLayer.children);
}

/**
 * @brief Interpolira replay stanje za zadani trenutak.
 * @param {number} timeMs Vrijeme u milisekundama od pocetka pokusaja.
 * @returns {object} Interpolirano stanje replaya.
 */
function interpolateEvent(timeMs) {
  if (!replay.length) {
    return { t: 0, fill: 0, target_index: 0, event: "empty" };
  }

  let previous = replay[0];
  let next = replay[replay.length - 1];

  for (let i = 1; i < replay.length; i += 1) {
    if (replay[i].t >= timeMs) {
      next = replay[i];
      break;
    }
    previous = replay[i];
  }

  if (next.t === previous.t) {
    return previous;
  }

  const progress = Math.max(0, Math.min(1, (timeMs - previous.t) / (next.t - previous.t)));
  return {
    t: timeMs,
    fill: Number(previous.fill) + (Number(next.fill) - Number(previous.fill)) * progress,
    target_index: previous.target_index,
    event: previous.event,
  };
}

/**
 * @brief Renderira replay marker, aktivni target i tekstualni readout.
 * @param {number} timeMs Vrijeme u milisekundama od pocetka pokusaja.
 */
function renderReplay(timeMs) {
  if (!replayStage || !elements.marker) return;

  const state = interpolateEvent(timeMs);
  const activeIndex = Number(state.target_index) || 0;
  const activeTargetFill = targets[activeIndex] ?? 0;

  elements.marker.style.left = fillToPercent(state.fill);
  elements.activeTarget.style.left = fillToPercent(activeTargetFill);
  elements.readout.textContent = `fill ${Number(state.fill).toFixed(2)} | target #${activeIndex + 1} | ${eventLabel(state.event)}`;

  elements.targets.forEach((targetNode) => {
    targetNode.classList.toggle("is-active", Number(targetNode.dataset.index) === activeIndex);
  });
}

/**
 * @brief Postavlja trenutno vrijeme replaya i sinkronizira scrub kontrolu.
 * @param {number} timeMs Novo vrijeme replaya u milisekundama.
 */
function setTime(timeMs) {
  const bounded = Math.max(0, Math.min(durationMs, timeMs));
  pausedAt = bounded;

  if (scrub) {
    scrub.value = durationMs ? Math.round((bounded / durationMs) * 1000) : 0;
  }

  if (timeLabel) {
    timeLabel.value = `${(bounded / 1000).toFixed(1)}s`;
  }

  renderReplay(bounded);
}

/**
 * @brief Animation-frame callback za automatsku reprodukciju replaya.
 * @param {DOMHighResTimeStamp} timestamp Vrijeme koje salje requestAnimationFrame.
 */
function tick(timestamp) {
  if (!startedAt) {
    startedAt = timestamp - pausedAt;
  }

  const elapsed = timestamp - startedAt;
  setTime(elapsed);

  if (elapsed >= durationMs) {
    playing = false;
    startedAt = 0;
    if (toggle) toggle.textContent = "Play";
    return;
  }

  animationFrame = requestAnimationFrame(tick);
}

if (toggle) {
  toggle.addEventListener("click", () => {
    if (!durationMs) return;

    playing = !playing;
    toggle.textContent = playing ? "Pause" : "Play";

    if (playing) {
      startedAt = 0;
      animationFrame = requestAnimationFrame(tick);
    } else {
      cancelAnimationFrame(animationFrame);
    }
  });
}

if (scrub) {
  scrub.addEventListener("input", () => {
    cancelAnimationFrame(animationFrame);
    playing = false;
    if (toggle) toggle.textContent = "Play";
    setTime(durationMs * (Number(scrub.value) / 1000));
  });
}

buildReplayStage();
setTime(0);
