# C++ And WebAssembly
This is an example C++ graphics application using vcpkg as its package manager; the codebase can be compiled for both native and Web platforms.
- Package Manager: vcpkg
- Supported Platforms: MacOSX, Web

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
cmake -S . -B build
cmake --build build

# Running
./build/main
```
#### Building for WebAssembly
```Shell
# Building
cmake -S . -B build-wasm -DEMSCRIPTEN=ON
cmake --build build-wasm

# Running (open http://localhost:3000/ in your browser)
python -m http.server -d ./build-wasm 3000 # Use your preferred local server
```

### Notes
- I didn't use GLEW because the vcpkg wasm32 port is currently borken