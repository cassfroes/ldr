Lode Runner — Web Port (prototype)

This folder contains a minimal prototype to load the original `LDDEF.FSE` level file and render the first level on an HTML5 Canvas.

Run locally with a static server (recommended):

```bash
# from this folder
npx http-server -c-1 .
# or
python3 -m http.server 8000
```

Open `http://localhost:8080/` (or the port used) and use the file input to load your `LDDEF.FSE` file from disk. The app will parse the first 32×23 bytes per level and draw tiles at 20×20 pixels.

Next steps:
- Implement sprite rendering and animation
- Implement game engine (player/enemy movement, digging, holes)
- Add keyboard controls and audio

