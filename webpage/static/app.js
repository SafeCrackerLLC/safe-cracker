/**
 * @file app.js
 * @brief Crta trend vremena i stabilnosti na leaderboard stranici.
 */

const canvas = document.getElementById("trend");
const rows = window.safeCrackerRecent || [];
const TREND_CANVAS_HEIGHT = 180;

/**
 * @brief Ponovno iscrta trend graf prema trenutnoj sirini canvasa.
 */
function drawTrend() {
  if (!canvas) return;

  const scale = window.devicePixelRatio || 1;
  const rect = canvas.getBoundingClientRect();
  const width = Math.max(1, rect.width);
  const cssHeight = TREND_CANVAS_HEIGHT;
  canvas.style.height = `${cssHeight}px`;
  canvas.width = Math.round(width * scale);
  canvas.height = Math.round(cssHeight * scale);

  const ctx = canvas.getContext("2d");
  ctx.setTransform(scale, 0, 0, scale, 0, 0);
  ctx.clearRect(0, 0, width, cssHeight);

  const height = cssHeight;
  const padding = 28;

  ctx.strokeStyle = "#263241";
  ctx.lineWidth = 1;
  for (let i = 0; i <= 4; i += 1) {
    const y = padding + ((height - padding * 2) * i) / 4;
    ctx.beginPath();
    ctx.moveTo(padding, y);
    ctx.lineTo(width - padding, y);
    ctx.stroke();
  }

  if (rows.length === 0) {
    ctx.fillStyle = "#94a3b8";
    ctx.font = "14px system-ui";
    ctx.fillText("No score data yet", padding, height / 2);
    return;
  }

  const maxTime = Math.max(...rows.map((row) => row.time_seconds), 1);
  const xFor = (index) => padding + ((width - padding * 2) * index) / Math.max(rows.length - 1, 1);
  const yTime = (row) => padding + (height - padding * 2) * (row.time_seconds / maxTime);
  const yStability = (row) => padding + (height - padding * 2) * (1 - row.stability / 100);

  drawLine(ctx, rows, xFor, yTime, "#f3b84a");
  drawLine(ctx, rows, xFor, yStability, "#39d98a");

  ctx.fillStyle = "#f3b84a";
  ctx.fillText("time", padding, 16);
  ctx.fillStyle = "#39d98a";
  ctx.fillText("stability", padding + 48, 16);
}

/**
 * @brief Crta liniju i tocke za jedan niz podataka.
 * @param {CanvasRenderingContext2D} ctx Canvas 2D kontekst.
 * @param {Array<object>} data Scoreovi koji se prikazuju.
 * @param {Function} xFor Funkcija koja indeks pretvara u X koordinatu.
 * @param {Function} yFor Funkcija koja redak pretvara u Y koordinatu.
 * @param {string} color CSS boja linije.
 */
function drawLine(ctx, data, xFor, yFor, color) {
  ctx.strokeStyle = color;
  ctx.lineWidth = 2;
  ctx.beginPath();
  data.forEach((row, index) => {
    const x = xFor(index);
    const y = yFor(row);
    if (index === 0) {
      ctx.moveTo(x, y);
    } else {
      ctx.lineTo(x, y);
    }
  });
  ctx.stroke();

  ctx.fillStyle = color;
  data.forEach((row, index) => {
    ctx.beginPath();
    ctx.arc(xFor(index), yFor(row), 3, 0, Math.PI * 2);
    ctx.fill();
  });
}

drawTrend();
window.addEventListener("resize", drawTrend);
