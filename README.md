# Vakarta - Vulkan Generative Art Sandbox
A generative art sandbox using the Vulkan API, and a home for my experimentations.

## Design goals
- Create robust/reusable abstractions around the Vulkan API.
- Learn about engine architecture.
- Experiment with cool algorithms.
- Implement a separated 'base' engine called VKRT. Which can be used for different generative art projects.

## Structure
- VKS
  - Abstractions to simplify my usage of Vulkan.
- VKRT
  - General engine code. Manages GUI, game loop, etc.
- MTTR
  - Home of different implementations using the VKRT engine.
 
### Implementations
- [Voxels](./mttr/voxels/README.md) - A voxel cellular automata
- [SCG](./mttr/scg/README.md) - A sacred geometry editor

## Building
VKRT requires the following packages to be available on your system:
- [Vulkan](https://www.vulkan.org)
- [spdlog](https://github.com/gabime/spdlog)
- [glfw](https://www.glfw.org)
- more?

Building happens using CMake:
- `mkdir build && cd build`
- `cmake ..`
- `make`

## Running
- Voxels: `build/matter/voxels/Voxels`

## Feedback
Do not hesitate to contact me with any feedback or ideas! I'm rather looking forward to improve my code style, readability, optimization, or just listen to your ideas.
You may always open an issue regarding this and I'll try to answer as soon as I see it.
