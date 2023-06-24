#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/application/Engine.h"

#define WIDTH 1600
#define HEIGHT 900
#define TITLE "VKRT"

int main()
{
    auto theWindow = std::make_shared< vks::Window >( WIDTH, HEIGHT, TITLE );
    auto theVulkanSession = vks::VksSession::GetInstance();
    vkrt::Engine engine = vkrt::Engine( theWindow, theVulkanSession );

    std::chrono::microseconds thePreviousFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    std::chrono::microseconds theFrameTime;

    spdlog::info("Started game loop");
    while( !engine.ShouldClose() )
    {
        thePreviousFrameTime = theFrameTime;
        theFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
        (
            std::chrono::system_clock::now().time_since_epoch()
        );
        float theFrameDuration = ( theFrameTime.count() - thePreviousFrameTime.count() ) % 10000000000 / 1000000.0f;

        engine.Update( theFrameDuration );
        engine.Render();
    }

    return EXIT_SUCCESS;
}
