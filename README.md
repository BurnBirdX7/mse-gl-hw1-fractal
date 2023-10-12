![build_cmake_status](https://github.com/BurnBirdX7/mse-gl-hw-fractal/actions/workflows/build_cmake.yml/badge.svg)

# ITMO MSE OpenGL homework #1 - Fractal

First HW

## Requirements

- C++17 compatible compiler;
- CMake 3.10+ [https://cmake.org/](https://cmake.org/);
- Qt 5 [https://www.qt.io/](https://www.qt.io/);
- (Optionally) Ninja build [https://ninja-build.org/](https://ninja-build.org/).

## Hardware requirements

- GPU with OpenGL 3+ support.

## Build from console

### Linux (if Qt can be found by CMake)

- Clone this repository `git clone <url> <path>`;
- Go to root folder `cd <path-to-repo-root>`;
- Launch `./build_and_run.sh`

### Manual

- Clone this repository `git clone <url> <path>`;
- Go to root folder `cd <path-to-repo-root>`;
- Create and go to build folder `mkdir -p build-release; cd build-release`;
- Run CMake `cmake .. -G <generator-name> -DCMAKE_PREFIX_PATH=<path-to-qt-installation> -DCMAKE_BUILD_TYPE=Release`;
- Run build. For Ninja generator it looks like `ninja -j<number-of-threads-to-build>`.

## Build with MSVC

- Clone this repository `git clone <url> <path>`;
- Open root folder in IDE;
- Build, possibly specify build configurations and path to Qt library.

## Run and debug

- Since we link with Qt dynamically don't forget to add `<qt-path>/<abi-arch>/bin` and `<qt-path>/<abi-arch>/plugins/platforms` to `PATH` variable.
