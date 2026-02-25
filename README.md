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

#### Requirements for WeatherApplet

* `curl` — fetching weather data with an API call
* `nlohmann-json` — parsing fetched weather data in JSON format

Additionally, an appropriate API key for OpenWeather is necessary for API calls

#### (optional) Requirements to run tests

* `gtest`

### Installing

*WORK IN PROGRESS*

#### Install the dependencies listed above

On Arch Linux

``` sh
sudo pacman -Sy qt6-base tomlplusplus curl nlohmann-json
```

#### Build the project

``` sh
cmake --preset Release .
cmake --build build/Release
```

#### Install the compiled project

*WORK IN PROGRESS* — `cmake --install` is not supported yet

#### Launch the executable

``` sh
./build/Release/PowerApplet
```

cmake --install is not supported yet

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

