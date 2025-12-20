# Robot Arm | WASM | QT | OpenGL

Leveraging the power of C++ and emscripten I managed to create ABI issues in JS. After being stuck in dependency and 
integration hell for quite a bit I finally managed to get this running.

---

## Features

- [x] A working build system

- [x] Extendable Robot Arm

- [x] Shader Configuration

- [ ] Making the arm toss something maybe

- [ ] Targets for the arm to hit


## How to build this


### Desktop 

For just the desktop application your life is easy, you'll need:

- CMake
- VCPKG
- Qt System Dependencies

Use the vcpkg presets and just wait for CMake to configure things out from there, then build.

```bash
cmake --preset dev-vcpkg
cmake --build build/dev-vcpkg
```

---

### WASM

For WASM one step made it quite a bit more involved for me but here's what you'll need

- CMake
- Emscripten (I used the arch packaged emscripten which caused multiple headaches)
- Host Qt install
- Qt WASM build (Can be downloaded, installed or built from source just make sure emscripten lines up)
- Alternatively you can figure out a better way to do this
- You'll also need to update the WASM preset to use your Qt WASM path

After that its basically the same as the desktop build.

```bash
cmake --preset dev-wasm
cmake --build build/dev-wasm
emrun build/dev-wasm/src/qt_test.html
```

--- 

## What I learned 

- I learned to appreciate VCPKG 
- WebGL ≠ OpenGL: no interface blocks in GLSL ES 3.0, different context rules
- `QOpenGLWindow` + `createWindowContainer()` actually works in WASM unlike `QOpenGLWidget` 
- The browser is quite quirky when it comes to mouse events and Qt

## What's missing

The architecture could be cleaner. The robot arm simulation lives inside the rendering code via model matrices, which works surprisingly well but doesn't scale. Future plans:

- Separate simulation from rendering
- `RenderQueue` abstraction with automatic instancing
- Texture/material support
- Cache uniform locations properly instead of querying every frame
- Splitting movement from rendering also allows for testing