#include "vkrt/Engine.h"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

Engine::Engine()
:
    mWindow( std::make_shared< vkrt::Window >( WIDTH, HEIGHT, TITLE ) ),
    mRenderer( mWindow )
{

}

Engine::~Engine()
{
}

// -------------------------------------------------------------------------------------------------------------------

void Engine::Update()
{
    mWindow->Poll();
}

void Engine::Render()
{
    mRenderer.Render();
}

bool Engine::ShouldClose()
{
    return mWindow->ShouldClose();
}

