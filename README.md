# LilyPi V1 - LVGL Demo

PlatformIO project for LilyPi V1 with LVGL demo widgets.

## Hardware

- **Board:** LilyPi V1 (ESP32-based)
- **Display:** ILI9481 (320x480, landscape mode)
- **Touch:** GT911 capacitive touch controller
- **Features:** SD card, RTC, relay control, USB control

## Features

✅ Full LVGL v7 demo widgets
✅ Landscape orientation (480x320)
✅ Touch input working
✅ Correct color display
✅ Hardware controls (relay, USB, brightness)

## Quick Start

### Prerequisites

- PlatformIO Core or VS Code with PlatformIO extension
- LilyPi V1 board

### Build

```bash
pio run -e esp32dev
```

### Upload

```bash
pio run -e esp32dev --target upload
```

Or use Arduino IDE to upload `.pio/build/esp32dev/firmware.bin`

## Configuration

The project uses:
- `LILYGO_LILYPI_V1` - Board definition
- `LILYGO_BLOCK_ILI9481_MODULE` - Display driver
- `LILYGO_WATCH_LVGL` - LVGL integration

Key initialization sequence:
```cpp
ttgo->begin();
ttgo->openBL();
ttgo->tft->setRotation(1);    // Landscape BEFORE lvgl_begin
ttgo->lvgl_begin();            // Initialize LVGL
lv_demo_widgets();             // Start demo
```

## Project Structure

```
├── include/
│   └── config.h              # Hardware configuration
├── src/
│   ├── main.cpp              # Main application
│   └── lv_demo_widgets.cpp   # LVGL demo
├── platformio.ini            # PlatformIO configuration
└── README.md
```

## Troubleshooting

See `SOLUTION.md` for detailed troubleshooting information.

## License

This project uses the TTGO TWatch Library which is licensed under MIT.

## Credits

- LVGL - https://lvgl.io/
- TTGO TWatch Library - https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library
