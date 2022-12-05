#include "vkrt/Engine.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Engine::Engine()
:
    mWindow( WIDTH, HEIGHT, TITLE ),
    mRenderer( mWindow.GetSurface() )
{

}

Engine::~Engine()
{
}

// -------------------------------------------------------------------------------------------------------------------

void Engine::Update()
{
    mWindow.Poll();
}

void Engine::Render()
{

}

bool Engine::ShouldClose()
{
    return mWindow.ShouldClose();
}

