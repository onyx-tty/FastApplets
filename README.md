# FastApplets

FastApplets is a lightweight suite of applets designed for high performance, simplicity, and efficiency

## Description

## Getting Started

### Dependencies

#### Requirements for all applets

* `qt6-base`
* `tomlplusplus`

#### Requirements for WeatherApplet

* `curl`
* `nlohmann-json`

Additionally, an appropriate API key for OpenWeather is necessary to fetch weather data

#### (optional) Requirements to run tests

* `gtest`

### Installing

#### Install the dependencies listed above

On Arch Linux

``` sh
sudo pacman -Sy qt6-base tomlplusplus curl nlohmann-json
```

#### Build

``` sh
cmake --preset Release .
cmake --build build/Release
```

#### Launch

``` sh
./build/Release/PowerApplet
```

The compiled executable can be linked, copied, or launched from the project root

Installation is not handled yet

## Help

## Authors

## Version History

## License

This project is licensed under the GPL 3.0 License - see the LICENSE.md file for details

## Acknowledgments

The README.md template
* [DomPizzie](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)

