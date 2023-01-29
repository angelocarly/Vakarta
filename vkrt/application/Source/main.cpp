#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/application/Engine.h"

int main()
{
    vkrt::Engine engine = vkrt::Engine();

    std::chrono::microseconds thePreviousFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    std::chrono::microseconds theFrameTime;

    try
    {
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
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
