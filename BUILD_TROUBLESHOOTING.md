# Build Troubleshooting Guide

## Current Issue: LEDC API and Library Compatibility

The TTGO TWatch Library has compatibility issues with different ESP32 Arduino Core versions.

## Solution Approaches

### Option 1: Use Platform espressif32@6.4.0 (Current)
This uses Arduino Core 2.0.11 which should have the old LEDC API that the library expects.

```ini
platform = espressif32@6.4.0
```

### Option 2: Use a Specific Working Configuration
Try this known working configuration:

```ini
[env:esp32dev]
platform = espressif32@5.4.0
board = esp32dev
framework = arduino

lib_deps = 
    https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library.git
    lvgl/lvgl@^7.11.0
```

### Option 3: Fork and Patch the Library
If the library continues to have issues:

1. Fork the TTGO_TWatch_Library repository
2. Update the LEDC API calls in `src/drive/tft/bl.h`:
   - Replace `ledcSetup()` with `ledcAttach()`
   - Replace `ledcAttachPin()` with `ledcAttach()`
   - Replace `ledcDetachPin()` with `ledcDetach()`
3. Update the pin definitions for LilyPi board
4. Use your forked version in `lib_deps`

### Option 4: Manual Library Patching

After the first failed build, manually patch the library:

```bash
# Navigate to the library
cd .pio/libdeps/esp32dev/TTGO\ TWatch\ Library/src/drive/tft/

# Edit bl.h to fix LEDC calls
# Then rebuild
pio run
```

## Pin Configuration Issues

The library may not have LilyPi V1 configuration. We've added:

```cpp
#define RELAY_PIN           25
#define EXTERN_USB_EN       26
```

You may need to add more pin definitions depending on errors.

## Alternative: Minimal Custom Implementation

If the library is too problematic, consider creating a minimal implementation:

1. Use TFT_eSPI library directly for display
2. Use LVGL directly without the TTGO wrapper
3. Implement relay/USB control with simple digitalWrite()
4. Skip SD card and RTC initially

Example platformio.ini for minimal approach:

```ini
[env:esp32dev]
platform = espressif32@6.4.0
board = esp32dev
framework = arduino

lib_deps = 
    bodmer/TFT_eSPI@^2.5.43
    lvgl/lvgl@^7.11.0

build_flags = 
    -DUSER_SETUP_LOADED=1
    -DILI9481_DRIVER=1
    -DTFT_WIDTH=320
    -DTFT_HEIGHT=480
    # Add your pin definitions here
```

## Checking Build Progress

The first build can take 5-10 minutes. To monitor:

```bash
# Run with verbose output
pio run -v

# Or check what's happening
ps aux | grep pio
```

## Common Errors and Fixes

### Error: `ledcSetup was not declared`
- **Cause**: Arduino Core 3.x removed old LEDC API
- **Fix**: Use platform espressif32@6.4.0 or earlier

### Error: `RELAY_PIN was not declared`
- **Cause**: Missing pin definitions for LilyPi V1
- **Fix**: Already added in `include/config.h`

### Error: Library download fails
- **Cause**: Network or Git issues
- **Fix**: Clone library manually to `lib/` folder

## Next Steps

1. Wait for current build to complete
2. Check error output
3. Apply appropriate fix from above options
4. Consider switching to minimal implementation if library proves too problematic
