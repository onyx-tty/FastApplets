# FastApplets

A collection of lightweight applets for system management, written in Qt6/C++

Currently only PowerApplet - more coming soon!

## Description

The applets are designed with focus on keyboard-first controls, but have full mouse support. To prevent accidental destructive actions, like shutdown or reboot, controlling buttons with keyboard requires pressing a button twice. Mouse presses don't have the same limitation.

### PowerApplet

Provides shutdown, reboot, suspend, and hibernate controls with customizable shell command passing.

## Who is this for?

Tiling window manager users who want lightweight utilities that complement their keyboard-centric workflow. Casual users are welcome too, mouse is also supported.

## Building & Installation

### Prerequisites

* `qt6-base`
* `qt6-svg`
* `tomlplusplus`
* `cmake`
* `ninja`
* `clang`

#### Arch Linux

``` sh
sudo pacman -Sy qt6-base qt6-svg tomlplusplus cmake ninja clang
```

#### Ubuntu

``` sh
sudo apt install qt6-base-dev libqt6svg6 libtomlplusplus-dev cmake ninja-build clang
```

### Build from source

``` sh
# Configure (static linking recommended)
cmake --preset Release

# Build
cmake --build --preset Release
```

**For dynamic linking**, pass `-DBUILD_SHARED_LIBS=ON` during configuration.
Make sure `/your/installation/path/lib` is in `$LD_LIBRARY_PATH`, so that shared libraries are found by the executable.

### Install

CPack is not supported at present; installation is manual.

``` sh
cmake --install build/Release --prefix /your/installation/path
```

### Run

``` sh
# If installed directory is in $PATH (e.g. /usr/bin)
PowerApplet

# Or with explicit path
/your/installation/path/bin/PowerApplet

# Or directly from build directory
./build/Release/src/Applets/PowerApplet/PowerApplet
```

### Test

``` sh
ctest --preset Debug --output-on-failure
```

## Authors

* [onyx-tty](https://github.com/onyx-tty)

## License

This project is licensed under the GPL 3.0 License — see the [LICENSE](./LICENSE) file for details

## Credits

Special thanks to C++, CMake, and Ninja for helping translate the code to binary every time I bump a copyright year.
