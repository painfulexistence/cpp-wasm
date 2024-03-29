# C++ And WebAssembly

[![Build](https://github.com/painfulexistence/cpp-wasm/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/painfulexistence/cpp-wasm/actions/workflows/ci.yml)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![WebAssembly](https://img.shields.io/badge/WebAssembly-654FF0?style=flat&logo=WebAssembly&logoColor=white)
[![Follow on Twitter](https://img.shields.io/twitter/follow/DevLucidum.svg?style=social)](https://twitter.com/intent/follow?screen_name=DevLucidum)
<br />
<br />

This is an example C++ graphics application using vcpkg as its package manager; the codebase can be compiled for both native and Web platforms.
- Package Manager: vcpkg
- Supported Platforms: MacOSX, Web
<br />

## Getting Started

### Requirements
- [CMake](https://cmake.org/download/)
- [EMSDK](https://emscripten.org/docs/getting_started/downloads.html)

### Getting the Source Code
```Shell
# Clone the repository
git clone --recurse-submodules https://github.com/painfulexistence/cpp-wasm && cd cpp-wasm

# Bootstrapping the package manager
./vcpkg/bootstrap-vcpkg.sh
```

### Building
#### Building for native
```Shell
# Building
cmake -S . -B build # Add -DCMAKE_BUILD_TYPE=release for release build
cmake --build build

# Running
./build/main
```
#### Building for WebAssembly
```Shell
# Building
cmake -S . -B build-wasm -DEMSCRIPTEN=ON # Add -DCMAKE_BUILD_TYPE=release for release build
cmake --build build-wasm

# Running (open http://localhost:3000/ in your browser)
python -m http.server -d ./build-wasm 3000 # Use your preferred local server
```
![Screenshot](https://github.com/painfulexistence/cpp-wasm/assets/30090734/8f56dd7d-2d28-42ac-ad11-69559ae4242a)

### Notes
- I didn't use GLEW because the vcpkg wasm32 port is currently broken
