# VKRT and cousins
A generative art sandbox using the Vulkan API.

## Design goals
- Create robust/reusable abstractions around the Vulkan API.
- Learn about engine architecture.
- Experiment with cool algorithms.
- Implement a separated 'base' engine called VKRT. Which can be used for different generative art projects.

## Structure
- VKS
  - Abstractions to simplify my usage of Vulkan.
- VKRT
  - General engine code. Host logic for frame representation and more to come.
- (VPS) need a new name
  - Home of different implementations using the VKRT engine.

## Feedback
Do not hesitate to contact me with any feedback or ideas! I'm rather looking forward to improve my code style, readability, optimization, or just listen to your ideas.
You may always open an issue regarding this and I'll try to answer as soon as I see it.

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
