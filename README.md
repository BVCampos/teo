# Teo - OpenGL glTF Renderer

A lightweight OpenGL 3.3 renderer that loads and displays glTF 2.0 models.

## Features

- OpenGL 3.3 Core profile rendering
- glTF 2.0 support (.gltf and .glb files)
- PBR base color textures
- Blinn-Phong lighting
- FPS camera controls

## Requirements

- CMake 3.20+
- SDL2
- OpenGL 3.3+ capable GPU

## Building

### macOS

```bash
# Install dependencies
brew install cmake sdl2

# Build
mkdir build && cd build
cmake ..
make

# Run
./teo ../path/to/model.gltf
```

### Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt install cmake libsdl2-dev

# Build
mkdir build && cd build
cmake ..
make

# Run
./teo ../path/to/model.gltf
```

### Windows

```bash
# Using vcpkg for dependencies
vcpkg install sdl2

# Build
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release

# Run
Release\teo.exe path\to\model.gltf
```

## Usage

```bash
./teo <model.gltf> [model2.glb] ...
```

### Controls

| Key | Action |
|-----|--------|
| W/A/S/D | Move camera |
| Mouse | Look around (click to capture) |
| Space | Move up |
| Shift | Move down |
| ESC | Release mouse / Exit |

## Sample Models

Download free glTF models from:
- [Khronos glTF Sample Models](https://github.com/KhronosGroup/glTF-Sample-Models)
- [Sketchfab](https://sketchfab.com/features/gltf)

```bash
# Example: Download and run the Duck model
curl -LO https://raw.githubusercontent.com/KhronosGroup/glTF-Sample-Models/main/2.0/Duck/glTF-Binary/Duck.glb
./teo Duck.glb
```

## Project Structure

```
teo/
├── src/
│   ├── main.cpp              # Entry point
│   ├── core/Window           # SDL2 window wrapper
│   ├── graphics/
│   │   ├── Shader            # GLSL shader management
│   │   ├── Mesh              # VAO/VBO geometry
│   │   ├── Texture           # Texture loading
│   │   └── Renderer          # Main render loop
│   ├── scene/
│   │   ├── Camera            # FPS camera
│   │   ├── Transform         # TRS transforms
│   │   └── Model             # Mesh collection
│   └── loader/GLTFLoader     # glTF parsing
├── shaders/
│   ├── basic.vert            # Vertex shader
│   └── basic.frag            # Fragment shader
└── third_party/
    ├── glad/                 # OpenGL loader
    └── tinygltf/             # glTF parser
```

## License

MIT
