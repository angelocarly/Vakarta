# VKRT
A generative art sandbox using the Vulkan API.

## Design goals
- Create robust/reusable abstractions around the Vulkan API.

## Structure
The project consists of a `vkrt` and a `vks` namespace. `vkrt` contains logic for the generative art whilst 'vks' contains abstractions around Vulkan. The abbreviations have no meaning yet.

## Documentation
- [Docs](./DOCS.md)

## Building
VKRT requires the following packages to be available on your system:
- [Vulkan](https://www.vulkan.org)
- [spdlog](https://github.com/gabime/spdlog)
- [glfw](https://www.glfw.org)

Building happens with CMake:
- `mkdir build && cd build`
- `cmake ..`
- `make`

## Running
- `build/vkrt/Debug/VKRT`

## Libraries
- [imnodes](https://github.com/Nelarius/imnodes)