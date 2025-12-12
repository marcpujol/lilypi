# LilyPi Simulator

SDL2-based simulator for testing the LilyPi interface on your development machine before deploying to hardware.

## Prerequisites

### macOS
```bash
brew install sdl2
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install SDL2-devel
```

## Building and Running

### Build the simulator
```bash
pio run -e simulator
```

### Run the simulator
```bash
.pio/build/simulator/program
```

Or build and run in one command:
```bash
pio run -e simulator -t exec
```

## Features

- **320x480 Display**: Matches the actual LilyPi hardware display size
- **Mouse Input**: Click and drag to simulate touch input
- **Same UI**: Runs the exact same `lv_demo_widgets()` as the hardware
- **Fast Iteration**: Test UI changes without uploading to device

## Project Structure

```
src/
├── simulator_main.c      # Simulator entry point
├── hal/
│   ├── hal.h            # HAL header
│   └── hal.c            # SDL2 HAL implementation
├── lv_demo_widgets.cpp  # Shared LVGL demo (used by both)
└── main.cpp             # ESP32 entry point (hardware)
```

## Simulator vs Hardware

| Feature | Simulator | Hardware |
|---------|-----------|----------|
| Display | SDL2 Window (320x480) | ILI9481 TFT (320x480) |
| Input | Mouse | GT911 Touch |
| Platform | native (PC) | ESP32-WROVER |
| Build Time | ~5 seconds | ~30 seconds |

## Troubleshooting

### SDL2 not found
If you see errors about SDL2 not being found:

1. **Check installation**:
   ```bash
   # macOS
   brew list sdl2
   
   # Linux
   dpkg -l | grep libsdl2-dev
   ```

2. **Verify sdl2-config**:
   ```bash
   which sdl2-config
   sdl2-config --version
   ```

3. **Manual SDL2 paths** (if needed):
   Edit `support/sdl2_build_extra.py` to add your SDL2 installation path.

### Build errors
```bash
# Clean and rebuild
pio run -e simulator -t clean
pio run -e simulator
```

### Window doesn't appear
- Check if SDL2 is properly installed
- Try running with verbose output: `pio run -e simulator -v`
- Check console for error messages

## Development Workflow

1. **Edit UI code** in `src/lv_demo_widgets.cpp`
2. **Test in simulator**: `pio run -e simulator -t exec`
3. **Verify changes** quickly without hardware
4. **Deploy to hardware**: `pio run -e esp32dev -t upload`

## Tips

- Use the simulator for rapid UI development and testing
- Test touch interactions with mouse clicks
- Simulator runs on your PC's CPU, so performance may differ from ESP32
- The simulator uses the same LVGL code, ensuring UI consistency

## Known Limitations

- No relay/USB control simulation
- No battery voltage reading
- No SD card access
- No RTC simulation
- Performance characteristics differ from actual hardware
