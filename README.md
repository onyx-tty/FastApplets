# FastApplets

A collection of lightweight applets for system management, written in Qt6/C++

Currently only PowerApplet - more coming soon!

## Description

The applets are designed with focus on keyboard-first controls, but with full mouse support. To prevent accidental destructive actions, like shutdown or reboot, they use a "stage-then-confirm" system: keyboard users must press twice. Mouse users don't have the same limitation.

### PowerApplet

Provides shutdown, reboot, suspend, and hibernate controls with customizable shell command passing.

## Who is this for?

Tiling window manager users who want lightweight, keyboard-driven utilities that don't fight their workflow. Casual users are welcome too, everything works with a mouse.

### Building & Installation

#### Prerequisites

* `qt6-base`
* `qt6-svg`
* `tomlplusplus`
* `cmake`
* `ninja`
* `clang`

##### Arch Linux

``` sh
sudo pacman -Sy qt6-base qt6-svg tomlplusplus cmake ninja clang
```

#### Build from source

``` sh
# Configure (static linking recommended)
cmake --preset Release

# Build
cmake --build --preset Release
```

**For dynamic linking**, pass `-DBUILD_SHARED_LIBS=ON` during configuration.
Make sure `/your/installation/path/lib` is in `$LD_LIBRARY_PATH`, so that shared libraries are found by the executable.

#### Install

CPack is not supported at present; installation is manual.

``` sh
cmake --install build/Release --prefix /your/installation/path
```

#### Run

``` sh
# If installed directory is in $PATH (e.g. /usr/bin)
PowerApplet

# Or with explicit path
/your/installation/path/bin/PowerApplet

# Or directly from build directory
./build/Release/src/Applets/PowerApplet/PowerApplet
```

#### Test

```sh
ctest --preset Debug --output-on-failure
```

## Background

This project started as a way to learn programming in C++17, including CMake, version control, writing tests, setting up CI, and ways to avoid never-ending refactors by structuring the code well. Eventually, it turned into something useful.

## Authors

* [onyx-tty](https://github.com/onyx-tty)

## License

This project is licensed under the GPL 3.0 License — see the [LICENSE](./LICENSE) file for details

## Credits

Special thanks to C++, CMake, and Ninja for helping translate the code to binary every time I bump a copyright year.
