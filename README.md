# OpenGL Project
Render a scene using OpenGL, loading vertices and faces from file and applying projection and gouraud shading.

## Requirements
- `CMake`
- `Make`

## Dependencies
- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de)
- [glm](https://github.com/g-truc/glm)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

## Configuration
The `config.yaml`file contains all the parameters used by the program, inclusing objects to render, their position, material, scale, etc.

## Controls
- `W`: Move forward
- `S`: Move backward
- `A`: Move left
- `D`: Move right
- `SPACE`: Move up
- `LEFT_SHIFT`: Move down
- `P`: Render in polygon mode
- `R`: Reload config file
- `ESC`: Exit

Moving the cursor while pressing the left mouse button will rotate the camera.

![Sample gif of the application](https://github.com/viniciusrsouza/cg-project-opengl/blob/main/images/sample.gif?raw=true)

## Build
```bash
# initialize submodules
git submodule init
git submodule update

# create build directory
mkdir build
cd build

# build
cmake ..
make
```

## Run
Inside the build folder, run `make run` or the executable `cgproject`.