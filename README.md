# FastApplets

FastApplets is a lightweight suite of applets designed for high performance, simplicity, and efficiency

## Description

*WORK IN PROGRESS*

## Getting Started

*WORK IN PROGRESS*

### Dependencies

*WORK IN PROGRESS*

#### Requirements for all applets

* `qt6-base`
* `tomlplusplus` — config files
* `cmake`
* `ninja`
* `clang` — the "linting" profile
* `gtest`

#### Requirements for WeatherApplet

* `curl` — fetching weather data with an API call
* `nlohmann-json` — parsing fetched weather data in JSON format

Additionally, an appropriate API key for OpenWeather is necessary for API calls

### Installing

*WORK IN PROGRESS*

#### Install the dependencies listed above

On Arch Linux

``` sh
sudo pacman -Sy qt6-base tomlplusplus cmake ninja clang gtest curl nlohmann-json
```

#### Configure the project

``` sh
# Static linking (recommended for regular users, if not packaging)
cmake --preset Release -DBUILD_SHARED_LIBS=OFF .

# Dynamic linking ($LD_LIBRARY_PATH must have access to libs at install location)
cmake --preset Release .
```

#### Build the project

``` sh
cmake --build build/Release
```

#### Install the compiled project

``` sh
# Install to a specific location
# WARNING: If linking dynamically, /your/installation/path/lib must be a part of
#          $LD_LIBRARY_PATH. The program won't be able to find the shared libraries
#          without it.
cmake --install build/Release --prefix /your/installation/path

# Install to the default prefix (build/{$presetName}/install)
cmake --install build/Release
```

#### Launch the executable

``` sh
# Run installed version
/your/installation/path/bin/PowerApplet

# Run without installation
./build/Release/src/Applets/PowerApplet/PowerApplet
```

## Help

*WORK IN PROGRESS*

## Authors

- [onyx-tty](https://github.com/onyx-tty)

## Version History

*AWAITING RELEASE*

## License

This project is licensed under the GPL 3.0 License — see the [LICENSE](./LICENSE) file for details

## Acknowledgments

Thanks to the following people for inspiration and resources:

- [DomPizzie](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc) — the README.md template

