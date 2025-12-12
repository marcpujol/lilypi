Import("env")
import os
import subprocess

# Get SDL2 configuration
def get_sdl2_config():
    try:
        # Try pkg-config first
        cflags = subprocess.check_output(['pkg-config', '--cflags', 'sdl2']).decode().strip().split()
        libs = subprocess.check_output(['pkg-config', '--libs', 'sdl2']).decode().strip().split()
        return cflags, libs
    except:
        # Fallback for macOS with Homebrew
        if os.path.exists('/opt/homebrew/bin/sdl2-config'):
            cflags = subprocess.check_output(['/opt/homebrew/bin/sdl2-config', '--cflags']).decode().strip().split()
            libs = subprocess.check_output(['/opt/homebrew/bin/sdl2-config', '--libs']).decode().strip().split()
            return cflags, libs
        # Fallback for macOS with standard Homebrew location
        elif os.path.exists('/usr/local/bin/sdl2-config'):
            cflags = subprocess.check_output(['/usr/local/bin/sdl2-config', '--cflags']).decode().strip().split()
            libs = subprocess.check_output(['/usr/local/bin/sdl2-config', '--libs']).decode().strip().split()
            return cflags, libs
        # Manual fallback
        else:
            print("Warning: SDL2 not found via pkg-config or sdl2-config. Please install SDL2:")
            print("  macOS: brew install sdl2")
            print("  Linux: sudo apt-get install libsdl2-dev")
            return [], []

# Apply SDL2 flags
cflags, libs = get_sdl2_config()

if cflags:
    env.Append(CFLAGS=cflags)
    env.Append(CXXFLAGS=cflags)

if libs:
    env.Append(LINKFLAGS=libs)

print("SDL2 configuration applied")
print(f"CFLAGS: {cflags}")
print(f"LIBS: {libs}")
