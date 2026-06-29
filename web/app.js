// Minimal prototype: load LDDEF.FSE (binary) and render first level on a 32x23 grid

const TILE_W = 20, TILE_H = 20;
const COLUMNS = 32, ROWS = 23;

// Tile constants (match LDR.C)
const TILE = {
  EMPTY: 0,
  TIJOLO: 1,
  CONCRETO: 2,
  TIJOLO_FALSO: 3,
  ESCADA: 4,
  BOLINHA: 5,
  ESCADAPASSAR: 6,
  PLR: 7,
  INI: 8,
  HOLE: 9
};

let tiles = new Array(COLUMNS);
for (let x = 0; x < COLUMNS; x++) tiles[x] = new Array(ROWS).fill(TILE.EMPTY);

const canvas = document.getElementById('game');
const ctx = canvas.getContext('2d');
ctx.imageSmoothingEnabled = false;
canvas.tabIndex = 0;
canvas.style.outline = 'none';
canvas.addEventListener('click', () => canvas.focus());

document.addEventListener('DOMContentLoaded', () => {
  canvas.focus();
  fetch('LDDEF.FSE')
    .then(response => {
      if (!response.ok) throw new Error(`Failed to load LDDEF.FSE: ${response.status}`);
      return response.arrayBuffer();
    })
    .then(buffer => {
      loadLevel(buffer);
      status.textContent = 'Loaded LDDEF.FSE automatically';
    })
    .catch(error => {
      console.warn(error);
      status.textContent = 'Load LDDEF.FSE manually if not found';
    });
});

const fileInput = document.getElementById('fileInput');
const status = document.getElementById('status');

const GAME_TICK_MS = 1000 / 60;
const MOVE_FRAMES = 8;
const HOLE_TICKS = 240;

let lastTimestamp = 0;
let accumulator = 0;
let gameStarted = false;

let player = {
  x: 0,
  y: 0,
  offsetX: 0,
  offsetY: 0,
  moving: false,
  frame: 0,
  dirX: 0,
  dirY: 0
};
let enemies = [];
let holes = [];
let points = 0;
let pressedKeys = new Set();
let levelBuffer = null;
let currentLevelIndex = 0;
let levelCount = 0;
let gameOver = false;
let levelCompleted = false;
let waitingForStart = false;
let exitVisible = false;

function parseLevel(arrayBuffer, levelIndex = 0) {
  const bytes = new Uint8Array(arrayBuffer);
  const needed = COLUMNS * ROWS;
  const offset = levelIndex * needed;
  if (bytes.length < offset + needed) {
    status.textContent = `Level data too small for level ${levelIndex + 1}`;
    return;
  }
  let k = 0;
  for (let x = 0; x < COLUMNS; x++) {
    for (let y = 0; y < ROWS; y++) {
      tiles[x][y] = bytes[offset + k++];
    }
  }
}

function tileAt(x, y) {
  if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS) return null;
  return tiles[x][y];
}

function isSolidTile(t) {
  return t === TILE.TIJOLO || t === TILE.CONCRETO || t === TILE.TIJOLO_FALSO;
}

function isClimbableTile(t) {
  return t === TILE.ESCADA || (t === TILE.ESCADAPASSAR && exitVisible);
}

function isPassableTile(t) {
  return t === TILE.EMPTY || t === TILE.BOLINHA || t === TILE.ESCADA || t === TILE.ESCADAPASSAR || t === TILE.HOLE;
}

function isEnemyOn(x, y) {
  return enemies.some(e => e.x === x && e.y === y);
}

function isStandingSupport(x, y) {
  const below = tileAt(x, y + 1);
  return below !== null && (
    isSolidTile(below)
    || below === TILE.ESCADAPASSAR
    || isClimbableTile(below)
    || (below === TILE.HOLE && isEnemyOn(x, y + 1))
  );
}

function canWalkTo(x, y) {
  const dest = tileAt(x, y);
  return dest !== null && isPassableTile(dest);
}

function canFallFrom(x, y) {
  const current = tileAt(x, y);
  if (current === TILE.ESCADA || current === TILE.ESCADAPASSAR) return false;
  if (current === TILE.HOLE && isEnemyOn(x, y)) return false;
  if (isStandingSupport(x, y)) return false;
  const below = tileAt(x, y + 1);
  return below !== null && isPassableTile(below);
}

function canEntityMove(entity, dx, dy) {
  const nx = entity.x + dx;
  const ny = entity.y + dy;
  const current = tileAt(entity.x, entity.y);
  const dest = tileAt(nx, ny);
  if (dest === null || !isPassableTile(dest)) return false;
  const currentHoleBlocked = current === TILE.HOLE && isEnemyOn(entity.x, entity.y);
  if (dx !== 0) {
    return isClimbableTile(current)
      || isStandingSupport(entity.x, entity.y)
      || currentHoleBlocked
      || isClimbableTile(dest)
      || (dest === TILE.HOLE && isEnemyOn(nx, ny));
  }
  if (dy < 0) {
    return isClimbableTile(current) || isClimbableTile(dest);
  }
  if (dy > 0) {
    return isClimbableTile(current) || isClimbableTile(dest) || isStandingSupport(nx, ny) || currentHoleBlocked;
  }
  return true;
}

function drawTile(x, y, t) {
  const sx = x * TILE_W;
  const sy = y * TILE_H;
  ctx.fillStyle = '#000';
  ctx.fillRect(sx, sy, TILE_W, TILE_H);
  switch (t) {
    case TILE.TIJOLO:
      ctx.fillStyle = '#8B4513'; ctx.fillRect(sx, sy, TILE_W, TILE_H);
      ctx.strokeStyle = '#D2691E'; ctx.strokeRect(sx, sy, TILE_W, TILE_H);
      break;
    case TILE.CONCRETO:
      ctx.fillStyle = '#A9A9A9'; ctx.fillRect(sx, sy, TILE_W, TILE_H);
      ctx.strokeStyle = '#777'; ctx.strokeRect(sx, sy, TILE_W, TILE_H);
      break;
    case TILE.ESCADA:
      ctx.fillStyle = '#000'; ctx.fillRect(sx, sy, TILE_W, TILE_H);
      ctx.strokeStyle = '#FFD700';
      ctx.beginPath();
      ctx.moveTo(sx + 4, sy); ctx.lineTo(sx + 4, sy + TILE_H);
      ctx.moveTo(sx + 14, sy); ctx.lineTo(sx + 14, sy + TILE_H);
      ctx.moveTo(sx + 2, sy); ctx.lineTo(sx + 16, sy);
      ctx.stroke();
      break;
    case TILE.BOLINHA:
      ctx.fillStyle = '#0A0';
      ctx.beginPath();
      ctx.ellipse(sx + 10, sy + 12, 4, 3, 0, 0, Math.PI * 2);
      ctx.fill();
      break;
    case TILE.ESCADAPASSAR:
      if (exitVisible) {
        ctx.fillStyle = '#000'; ctx.fillRect(sx, sy, TILE_W, TILE_H);
        ctx.strokeStyle = '#66CCFF';
        ctx.beginPath();
        ctx.moveTo(sx + 4, sy); ctx.lineTo(sx + 4, sy + TILE_H);
        ctx.moveTo(sx + 14, sy); ctx.lineTo(sx + 14, sy + TILE_H);
        ctx.stroke();
      }
      break;
    case TILE.HOLE:
      ctx.fillStyle = '#000'; ctx.fillRect(sx, sy, TILE_W, TILE_H);
      ctx.strokeStyle = '#333'; ctx.strokeRect(sx + 4, sy + 4, TILE_W - 8, TILE_H - 8);
      ctx.fillStyle = '#111'; ctx.fillRect(sx + 6, sy + 6, TILE_W - 12, TILE_H - 12);
      break;
    default:
      break;
  }
}

function drawPlayer() {
  const sx = player.x * TILE_W + player.offsetX;
  const sy = player.y * TILE_H + player.offsetY;
  ctx.fillStyle = player.frame % 16 < 8 ? '#ffffff' : '#ccf';
  ctx.fillRect(sx + 4, sy + 2, TILE_W - 8, TILE_H - 4);
  ctx.fillStyle = '#000';
  ctx.fillRect(sx + 7, sy + 6, 2, 2);
  ctx.fillRect(sx + 11, sy + 6, 2, 2);
  ctx.beginPath();
  ctx.arc(sx + 10, sy + 12, 2, 0, Math.PI * 2);
  ctx.fill();
}

function drawEnemy(enemy) {
  const sx = enemy.x * TILE_W + enemy.offsetX;
  const sy = enemy.y * TILE_H + enemy.offsetY;
  ctx.fillStyle = enemy.frame % 20 < 10 ? '#ff6666' : '#ff3333';
  ctx.fillRect(sx + 4, sy + 4, TILE_W - 8, TILE_H - 8);
  ctx.fillStyle = '#000';
  ctx.fillRect(sx + 6, sy + 7, 3, 3);
  ctx.fillRect(sx + 13, sy + 7, 3, 3);
}

function render() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  for (let y = 0; y < ROWS; y++) {
    for (let x = 0; x < COLUMNS; x++) {
      drawTile(x, y, tiles[x][y]);
    }
  }
  enemies.forEach(drawEnemy);
  drawPlayer();
  if (gameOver || levelCompleted || waitingForStart) {
    ctx.fillStyle = 'rgba(0, 0, 0, 0.65)';
    ctx.fillRect(0, 0, canvas.width, canvas.height);
    ctx.fillStyle = '#fff';
    ctx.font = '20px Arial';
    ctx.textAlign = 'center';
    if (gameOver) {
      ctx.fillText('Game Over', canvas.width / 2, canvas.height / 2 - 10);
      ctx.font = '14px Arial';
      ctx.fillText('Press R to retry', canvas.width / 2, canvas.height / 2 + 20);
    } else if (levelCompleted) {
      ctx.fillText('Level Complete', canvas.width / 2, canvas.height / 2 - 10);
      ctx.font = '14px Arial';
      ctx.fillText('Press R to retry', canvas.width / 2, canvas.height / 2 + 20);
    } else {
      ctx.fillText('Press any key to start', canvas.width / 2, canvas.height / 2 - 10);
    }
  }
}

function collectTile(x, y) {
  if (tiles[x][y] === TILE.BOLINHA) {
    tiles[x][y] = TILE.EMPTY;
    points += 30;
    status.textContent = `Points: ${points}`;
    if (isLevelCleared()) {
      exitVisible = true;
      status.textContent = 'All balls collected! Blue ladders appear.';
    }
  }
}

function loadLevelByIndex(index) {
  if (!levelBuffer) return;
  if (index < 0 || index >= levelCount) return;
  currentLevelIndex = index;
  exitVisible = false;
  waitingForStart = index > 0;
  levelCompleted = false;
  gameOver = false;
  parseLevel(levelBuffer, index);
  initEntities();
  render();
  if (waitingForStart) {
    status.textContent = 'Press any key to start';
  } else {
    startGame();
  }
}

function isLevelCleared() {
  for (let y = 0; y < ROWS; y++) {
    for (let x = 0; x < COLUMNS; x++) {
      if (tiles[x][y] === TILE.BOLINHA) return false;
    }
  }
  return true;
}

function setGameOver(message) {
  gameOver = true;
  status.textContent = message;
}

function restartLevel() {
  if (!levelBuffer) return;
  levelCompleted = false;
  waitingForStart = false;
  gameOver = false;
  loadLevelByIndex(currentLevelIndex);
}

function checkPlayerEnemyCollision() {
  if (enemies.some(e => e.x === player.x && e.y === player.y)) {
    setGameOver('Game Over - caught by enemy');
  }
}

function checkLevelComplete() {
  if (!levelCompleted && exitVisible && player.y === 0) {
    if (currentLevelIndex + 1 < levelCount) {
      loadLevelByIndex(currentLevelIndex + 1);
    } else {
      levelCompleted = true;
      waitingForStart = true;
      status.textContent = 'All levels complete! Press any key to restart';
    }
  }
}

function startMove(entity, dx, dy) {
  entity.targetX = entity.x + dx;
  entity.targetY = entity.y + dy;
  entity.dirX = dx;
  entity.dirY = dy;
  entity.moving = true;
}

function finishMove(entity) {
  entity.x = entity.targetX;
  entity.y = entity.targetY;
  entity.offsetX = 0;
  entity.offsetY = 0;
  entity.moving = false;
}

function digHole(dx) {
  const hx = player.x + dx;
  const hy = player.y + 1;
  const canDig = tileAt(hx, hy) === TILE.TIJOLO && canWalkTo(player.x, player.y);
  console.log('digHole', dx, hx, hy, tileAt(hx, hy), 'canWalk', canWalkTo(player.x, player.y), 'canDig', canDig);
  if (canDig) {
    tiles[hx][hy] = TILE.HOLE;
    holes.push({ x: hx, y: hy, ticks: HOLE_TICKS });
    status.textContent = 'Hole dug';
  } else {
    status.textContent = 'Cannot dig here';
  }
}

function updatePlayer() {
  player.frame += 1;
  if (!player.moving) {
    if (canFallFrom(player.x, player.y)) {
      console.log('player falling from', player.x, player.y);
      startMove(player, 0, 1);
      return;
    }
    const nextKey = ['ArrowLeft', 'ArrowRight', 'ArrowUp', 'ArrowDown'].find(k => pressedKeys.has(k));
    if (nextKey) {
      const moveMap = {
        ArrowLeft: [-1, 0],
        ArrowRight: [1, 0],
        ArrowUp: [0, -1],
        ArrowDown: [0, 1]
      };
      const [dx, dy] = moveMap[nextKey];
      const canMove = canEntityMove(player, dx, dy);
      console.log('player move check', nextKey, dx, dy, canMove, 'current', player.x, player.y, tileAt(player.x, player.y), 'dest', tileAt(player.x + dx, player.y + dy));
      if (canMove) {
        console.log('player start move', dx, dy);
        startMove(player, dx, dy);
      }
    }
    return;
  }

  player.offsetX += player.dirX * TILE_W / MOVE_FRAMES;
  player.offsetY += player.dirY * TILE_H / MOVE_FRAMES;
  if (Math.abs(player.offsetX) >= TILE_W || Math.abs(player.offsetY) >= TILE_H) {
    finishMove(player);
    collectTile(player.x, player.y);
  }
}

function getEnemyPathStep(enemy) {
  const start = { x: enemy.x, y: enemy.y };
  const target = { x: player.x, y: player.y };
  const queue = [start];
  const prev = Array.from({ length: COLUMNS }, () => Array(ROWS).fill(null));
  const visited = Array.from({ length: COLUMNS }, () => Array(ROWS).fill(false));
  visited[start.x][start.y] = true;

  while (queue.length) {
    const pos = queue.shift();
    if (pos.x === target.x && pos.y === target.y) break;
    const currentTile = tileAt(pos.x, pos.y);
    const below = tileAt(pos.x, pos.y + 1);
    const standing = isClimbableTile(currentTile) || (below !== null && !isPassableTile(below));

    const candidates = [
      { dx: -1, dy: 0 },
      { dx: 1, dy: 0 },
      { dx: 0, dy: -1 },
      { dx: 0, dy: 1 }
    ];

    for (const { dx, dy } of candidates) {
      if (!canEntityMove(pos, dx, dy)) continue;
      const nx = pos.x + dx;
      const ny = pos.y + dy;
      if (nx < 0 || nx >= COLUMNS || ny < 0 || ny >= ROWS) continue;
      if (visited[nx][ny]) continue;
      visited[nx][ny] = true;
      prev[nx][ny] = pos;
      queue.push({ x: nx, y: ny });
    }
  }

  if (!visited[target.x][target.y]) return null;
  let cur = target;
  while (prev[cur.x][cur.y] && !(prev[cur.x][cur.y].x === start.x && prev[cur.x][cur.y].y === start.y)) {
    cur = prev[cur.x][cur.y];
  }
  return { dx: cur.x - start.x, dy: cur.y - start.y };
}

function updateEnemy(enemy) {
  enemy.frame += 1;
  if (!enemy.moving) {
    if (canFallFrom(enemy.x, enemy.y)) {
      startMove(enemy, 0, 1);
      return;
    }
    const step = getEnemyPathStep(enemy);
    if (step) {
      startMove(enemy, step.dx, step.dy);
    } else {
      const deltaX = Math.sign(player.x - enemy.x);
      const deltaY = Math.sign(player.y - enemy.y);
      if (deltaX !== 0 && canEntityMove(enemy, deltaX, 0)) {
        startMove(enemy, deltaX, 0);
      } else if (deltaY !== 0 && canEntityMove(enemy, 0, deltaY)) {
        startMove(enemy, 0, deltaY);
      }
    }
    return;
  }

  enemy.offsetX += enemy.dirX * TILE_W / MOVE_FRAMES;
  enemy.offsetY += enemy.dirY * TILE_H / MOVE_FRAMES;
  if (Math.abs(enemy.offsetX) >= TILE_W || Math.abs(enemy.offsetY) >= TILE_H) {
    finishMove(enemy);
  }
}

function updateHoles() {
  for (let i = holes.length - 1; i >= 0; i--) {
    const hole = holes[i];
    hole.ticks -= 1;
    if (hole.ticks <= 0) {
      const occupied = (player.x === hole.x && player.y === hole.y) || enemies.some(e => e.x === hole.x && e.y === hole.y);
      if (!occupied && tiles[hole.x][hole.y] === TILE.HOLE) {
        tiles[hole.x][hole.y] = TILE.TIJOLO;
        holes.splice(i, 1);
      } else {
        hole.ticks = 10;
      }
    }
  }
}

function updateGame() {
  if (gameOver || waitingForStart) return;
  updatePlayer();
  enemies.forEach(updateEnemy);
  checkPlayerEnemyCollision();
  updateHoles();
  checkLevelComplete();
}

function gameLoop(timestamp) {
  if (!lastTimestamp) lastTimestamp = timestamp;
  const delta = timestamp - lastTimestamp;
  lastTimestamp = timestamp;
  accumulator += delta;
  while (accumulator >= GAME_TICK_MS) {
    updateGame();
    accumulator -= GAME_TICK_MS;
  }
  render();
  requestAnimationFrame(gameLoop);
}

function initEntities() {
  enemies = [];
  holes = [];
  points = 0;
  player.offsetX = 0;
  player.offsetY = 0;
  player.moving = false;
  player.frame = 0;
  player.dirX = 0;
  player.dirY = 0;

  for (let y = 0; y < ROWS; y++) {
    for (let x = 0; x < COLUMNS; x++) {
      const t = tiles[x][y];
      if (t === TILE.PLR) {
        player.x = x;
        player.y = y;
        player.targetX = x;
        player.targetY = y;
        tiles[x][y] = TILE.EMPTY;
      }
      if (t === TILE.INI) {
        enemies.push({ x, y, targetX: x, targetY: y, offsetX: 0, offsetY: 0, moving: false, frame: 0, dirX: 0, dirY: 0 });
        tiles[x][y] = TILE.EMPTY;
      }
    }
  }
}

function loadLevel(arrayBuffer) {
  levelBuffer = arrayBuffer.slice(0);
  const needed = COLUMNS * ROWS;
  levelCount = Math.max(1, Math.floor(levelBuffer.byteLength / needed));
  currentLevelIndex = 0;
  waitingForStart = false;
  loadLevelByIndex(0);
}

function startGame() {
  if (!gameStarted) {
    gameStarted = true;
    requestAnimationFrame(gameLoop);
  }
}

window.addEventListener('keydown', (ev) => {
  if (['ArrowLeft', 'ArrowRight', 'ArrowUp', 'ArrowDown', 'z', 'Z', 'x', 'X', '+', '=', '-', 'r', 'R'].includes(ev.key)) {
    ev.preventDefault();
  }
  pressedKeys.add(ev.key);
  console.log('keydown', ev.key);
  if (waitingForStart && !gameOver) {
    waitingForStart = false;
    status.textContent = `Game started`; 
    startGame();
    return;
  }
  status.textContent = `Key pressed: ${ev.key}`;
  switch (ev.key) {
    case 'z':
    case 'Z':
      digHole(-1);
      break;
    case 'x':
    case 'X':
      digHole(1);
      break;
    case 'r':
    case 'R':
      restartLevel();
      break;
    case '+':
    case '=':
      status.textContent = 'Speed +';
      break;
    case '-':
      status.textContent = 'Speed -';
      break;
    case 'Escape':
      status.textContent = 'Menu/Esc pressed';
      break;
  }
});

window.addEventListener('keyup', (ev) => {
  pressedKeys.delete(ev.key);
  console.log('keyup', ev.key);
});

window.addEventListener('blur', () => {
  pressedKeys.clear();
});

fileInput.addEventListener('change', e => {
  const f = e.target.files[0];
  if (!f) return;
  const reader = new FileReader();
  reader.onload = () => {
    loadLevel(reader.result);
    status.textContent = `Loaded ${f.name}`;
    canvas.focus();
  };
  reader.readAsArrayBuffer(f);
});

window.tiles = tiles;
window.parseLevel = parseLevel;
window.renderLevel = render;
window.loadLevel = loadLevel;

console.log('Lode Runner web prototype ready. Load LDDEF.FSE using the file input.');
