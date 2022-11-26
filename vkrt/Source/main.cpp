#include <iostream>
#include <cstdlib>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "vkrt/Engine.h"
#include "vks/Instance.h"

void setup_logging();

int main()
{

    vks::Instance& a = vks::Instance::GetInstance();//.GetVulkanInstance();
    a.GetVulkanInstance();

    Engine engine = Engine();

    try
    {
        spdlog::info("Started");
        while( !engine.shouldClose() )
        {
            engine.update();
            engine.render();
        }
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
