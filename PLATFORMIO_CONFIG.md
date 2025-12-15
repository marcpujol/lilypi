# LilyPi V1 - PlatformIO Configuration Summary

## Hardware Specifications (from board-specs.txt)
- **MCU**: ESP32-WROVER
- **Flash**: 8MB
- **PSRAM**: 16MB
- **Display**: 3.5" ILI9481 with GT911 touch controller
- **USB-TTL**: CH9102F
- **RTC**: PCF8563
- **Relay**: HFD3 5V 2A
- **Battery**: 3.7V Li-ion (IO35 for monitoring)
- **I2C**: SDA=IO21, SCL=IO22, INT=IO34

## PlatformIO Configuration

### Platform & Board
```ini
platform = espressif32@6.4.0
board = esp32dev
framework = arduino
```

### Memory Configuration
```ini
board_build.flash_size = 8MB
board_upload.flash_size = 8MB
board_build.partitions = default.csv
```

### CPU & Flash Settings
```ini
board_build.f_cpu = 240000000L        # 240 MHz
board_build.f_flash = 80000000L       # 80 MHz
board_build.flash_mode = qio          # Quad I/O
```

### Build Flags
```ini
-DBOARD_HAS_PSRAM                     # Enable PSRAM support
-mfix-esp32-psram-cache-issue         # PSRAM cache bug workaround
-DLILYGO_LILYPI_V1                    # Hardware selection
-DLILYGO_BLOCK_ILI9481_MODULE         # Display driver selection
```

### Libraries
- **TTGO TWatch Library**: Includes hardware drivers and LVGL v7
  - Source: https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library.git
  - Provides: Display, touch, RTC, SD card, relay, USB control

### Monitor Settings
```ini
monitor_speed = 115200
monitor_filters = esp32_exception_decoder
```

## Hardware Configuration (config.h)

```cpp
#define LILYGO_LILYPI_V1                // Board selection
#define LILYGO_BLOCK_ILI9481_MODULE     // Display driver
#define LILYGO_WATCH_LVGL               // Enable LVGL
#include <LilyGoWatch.h>
```

## Display Configuration

### Landscape Mode Setup (in main.cpp)
```cpp
ttgo->tft->setRotation(1);              // Set rotation BEFORE LVGL
ttgo->lvgl_begin();                     // Auto-detects rotation
```

### Display Specifications
- **Resolution**: 320x480 (portrait), 480x320 (landscape)
- **Driver**: ILI9481
- **Touch Controller**: GT911 (capacitive)
- **Interface**: SPI

## Project Structure
```
lilypi/
├── platformio.ini          # PlatformIO configuration
├── include/
│   └── config.h           # Hardware configuration
├── src/
│   ├── main.cpp           # Main application
│   ├── lv_demo_widgets.cpp # LVGL demo
│   └── lv_demo_widgets.h
└── board-specs.txt        # Hardware specifications
```

## Hardware Features Available

### TTGOClass API Functions
- `ttgo->begin()` - Initialize hardware
- `ttgo->openBL()` - Turn on backlight
- `ttgo->setBrightness(level)` - Adjust brightness (0-255)
- `ttgo->turnOnRelay()` / `ttgo->turnOffRelay()` - Relay control
- `ttgo->turnOnUSB()` / `ttgo->turnOffUSB()` - USB power control
- `ttgo->getVoltage()` - Battery voltage monitoring
- `ttgo->sdcard_begin()` - SD card initialization
- `ttgo->deviceProbe(addr)` - I2C device detection
- `ttgo->tft` - TFT display access
- `ttgo->lvgl_begin()` - LVGL initialization

### Pin Mappings (from library)
- **Battery ADC**: GPIO35
- **Touch I2C**: SDA=GPIO21, SCL=GPIO22, INT=GPIO34
- **Relay**: Controlled via TTGOClass
- **USB Power**: Controlled via TTGOClass

## Building & Uploading

### Commands
```bash
# Build the project
pio run

# Upload to board
pio run --target upload

# Open serial monitor
pio device monitor

# Clean build
pio run --target clean

# Build and upload and monitor
pio run --target upload && pio device monitor
```

### Upload Settings
```ini
upload_speed = 115200
```

## Verification Checklist

✅ ESP32-WROVER chip support
✅ 8MB Flash configuration
✅ 16MB PSRAM enabled with cache fix
✅ ILI9481 display driver
✅ GT911 touch controller
✅ CH9102F USB-TTL (auto-detected)
✅ 240MHz CPU frequency
✅ QIO flash mode for optimal performance
✅ LVGL integration via TTGO library
✅ Landscape orientation (480x320)
✅ Hardware-specific build flags
✅ Exception decoder for debugging

## Notes

1. **PSRAM Cache Issue**: The `-mfix-esp32-psram-cache-issue` flag is critical for ESP32-WROVER stability
2. **Display Rotation**: Must be set BEFORE calling `lvgl_begin()` for proper coordinate mapping
3. **LVGL Version**: The TTGO library includes LVGL v7.x
4. **Simulator Environment**: A separate `[env:simulator]` configuration exists for desktop testing
5. **Build Flags**: Hardware defines in both `platformio.ini` and `config.h` ensure proper compilation

## Status

✅ **Project is properly configured for LilyPi V1 board**

All specifications from board-specs.txt are correctly reflected in the PlatformIO configuration.
