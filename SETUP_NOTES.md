# Setup Notes

## Python Version Issue

The current system has Python 3.14.0, but PlatformIO's ESP32 platform requires Python 3.10-3.13.

### Solutions:

1. **Use pyenv to install a compatible Python version:**
   ```bash
   pyenv install 3.13.0
   pyenv local 3.13.0
   ```

2. **Or use a virtual environment with compatible Python:**
   ```bash
   python3.13 -m venv venv
   source venv/bin/activate
   pip install platformio
   ```

3. **Check PlatformIO Python version:**
   ```bash
   pio system info
   ```

## Building the Project

Once Python version is compatible:

```bash
# Clean build
pio run --target clean

# Build project
pio run

# Upload to board
pio run --target upload

# Monitor serial output
pio device monitor
```

## Troubleshooting

If you encounter library issues:
```bash
# Update libraries
pio lib update

# Install libraries manually
pio lib install
```

## Hardware Connections

Make sure the board is connected via USB Type-C and the correct serial port is detected by PlatformIO.
