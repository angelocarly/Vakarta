# Roadmap
List of planned features in chronological order

1. Get a basic Vulkan mWindow up and running
2. Design an abstraction around Imgui windows
3. Shader abstractions + Writing raytracing shaders

## Get a basic Vulkan mWindow up and running
Build a small abstraction around Vulkan. Also Experiment with the calls, get a better feeling for them.

- Implement a rendering system, possibly abstracted from application logic
- Full windowing support
- Extensible

Think about a good structure to organize my code.

## Design an abstraction around ImGui
Study ImGui, how does it handle windows, how to have a customizable i/o system?  
Extend Vulkan with this system, best case abstracted as to support different GUI systems as well.

## Shader abstractions + writing raytracing shaders
Fun part!

Make sure I don't dig a hole of technical debt. Write an abstraction for shaders that can extended.
Plugging in GUI sliders would be great.