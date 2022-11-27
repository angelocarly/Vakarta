#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>

#include "vkrt/Engine.h"

int main()
{
    Engine engine = Engine();

    try
    {
        spdlog::info("Started game loop");
        while( !engine.ShouldClose() )
        {
            engine.Update();
            engine.Render();
        }
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
