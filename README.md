# C++ And WebAssembly
Package Manager: vcpkg
Supported Platforms: MacOSX

## Getting Started

### Requirements
- [CMake](https://cmake.org/download/)
- [EMSDK](https://emscripten.org/docs/getting_started/downloads.html)

### Getting the Source Code
```Shell
# Clone the repository
git clone --recurse-submodules {REPO_URL} && cd cpp-wasm
# Bootstrapping the package manager
./vcpkg/bootstrap-vcpkg.sh
```

### Building
#### Building for native
```Shell
# Building
cmake -S . -B build
cmake --build build

# Running
./build/cpp-wasm
```
#### Building for WebAssembly
```Shell
# Building
cmake -S . -B build-wasm -DEMSCRIPTEN=ON
cmake --build build-wasm

# Running (open http://localhost:3000/ in your browser)
python -m http.server -d ./build-wasm 3000 # Use your preferred local server
```
#### Notes
We didn't use GLEW because the vcpkg wasm32 port is currently borken