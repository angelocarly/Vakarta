//
// Created by magnias on 7/7/22.
//

#include "vks/ForwardDecl.h"
#include "vks/Instance.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vulkan/vulkan.hpp>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.hpp>

// ============================================ Debug Callback =========================================================

VkResult
vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void
vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    switch( messageSeverity )
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            spdlog::get( "vulkan" )->debug( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            spdlog::get( "vulkan" )->info( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            spdlog::get( "vulkan" )->warn( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            spdlog::get( "vulkan" )->error( "Validation layer: {}", pCallbackData->pMessage );
            break;
    }
    return VK_FALSE;
}

// =====================================================================================================================

class vks::Instance::Impl
{
    public:
        Impl();

        void CreateVulkanInstance();
        bool CheckValidationLayersSupport();
        void SetupDebugMessenger();

        void DestroyDebugMessenger();
        void DestroyVulkanInstance();

        std::vector< const char * > GetRequiredExtensions();

    private:
        void GetDebugUtilsMessengerCreateInfo( vk::DebugUtilsMessengerCreateInfoEXT & inCreateInfo );

    public:
#ifdef NDEBUG
        const bool kEnableValidationLayers = false;
#else
        const bool kEnableValidationLayers = true;
#endif

    public:
        vk::Instance mInstance;
        vk::DebugUtilsMessengerEXT mDebugMessenger;

        const std::vector< const char * > mValidationLayers =
        {
            "VK_LAYER_KHRONOS_validation"
        };

};

// ---------------------------------------------------------------------------------------------------------------------

vks::Instance::Impl::Impl()
:
    mInstance()
{
}

void
vks::Instance::Impl::GetDebugUtilsMessengerCreateInfo( vk::DebugUtilsMessengerCreateInfoEXT & inCreateInfo )
{
    inCreateInfo.flags = vk::DebugUtilsMessengerCreateFlagsEXT();
    inCreateInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    inCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    inCreateInfo.pfnUserCallback = &DebugCallback;
}


void
vks::Instance::Impl::CreateVulkanInstance()
{
    vk::ApplicationInfo applicationInfo = vk::ApplicationInfo
    (
        "VKRT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        "VKRT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        VK_API_VERSION_1_3
    );

    vk::DebugUtilsMessengerCreateInfoEXT theDebugUtilsMessengerCreateInfo;
    GetDebugUtilsMessengerCreateInfo( theDebugUtilsMessengerCreateInfo );

    auto requiredExtensions = GetRequiredExtensions();
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo
    (
        vk::InstanceCreateFlags( vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR ),
        &applicationInfo,
        mValidationLayers.size(),
        mValidationLayers.data(),
        requiredExtensions.size(),
        requiredExtensions.data(),
        &theDebugUtilsMessengerCreateInfo
    );

    mInstance = vk::createInstance( createInfo );
}

std::vector< const char * >
vks::Instance::Impl::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> requiredExtensions;
    for(uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions.emplace_back(glfwExtensions[i]);
    }
    requiredExtensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );
    requiredExtensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

    return requiredExtensions;
}

bool
vks::Instance::Impl::CheckValidationLayersSupport()
{
    std::vector< vk::LayerProperties > theLayerProperties = vk::enumerateInstanceLayerProperties();
    for( const char * theRequestedLayer : mValidationLayers )
    {
        bool layerIsFound = false;
        for( vk::LayerProperties theProperty : theLayerProperties )
        {
            if( theProperty.layerName == theRequestedLayer )
            {
                layerIsFound = true;
                break;
            }
        }

        if( layerIsFound )
        {
            spdlog::get("vulkan")->error( "Requested layer not found: {}", theRequestedLayer );
            std::exit( 1 );
        }
    }
}

void
vks::Instance::Impl::SetupDebugMessenger()
{
    if( !kEnableValidationLayers ) return;

    vk::DebugUtilsMessengerCreateInfoEXT theCreateInfo;
    GetDebugUtilsMessengerCreateInfo( theCreateInfo );

    mDebugMessenger = mInstance.createDebugUtilsMessengerEXT( theCreateInfo );
}

void vks::Instance::Impl::DestroyDebugMessenger()
{
    mInstance.destroyDebugUtilsMessengerEXT( mDebugMessenger );
}

void vks::Instance::Impl::DestroyVulkanInstance()
{
    vkDestroyInstance( mInstance, nullptr );
}

// =====================================================================================================================

vks::Instance::Instance()
:
    mImpl( new Impl() )
{
    // Setup general vulkan logging
    spdlog::set_level(spdlog::level::debug);
    spdlog::stdout_color_mt("vulkan");

    // Setup intance and utilities
    mImpl->CreateVulkanInstance();
    mImpl->SetupDebugMessenger();
    mImpl->CheckValidationLayersSupport();
}

vks::Instance::~Instance()
{
    spdlog::get( "vulkan" )->debug( "Destroying instance" );
    mImpl->DestroyDebugMessenger();
    mImpl->DestroyVulkanInstance();
}

// ---------------------------------------------------------------------------------------------------------------------

vks::Instance &
vks::Instance::GetInstance()
{
    static vks::Instance vksInstance;
    return vksInstance;
}

vk::Instance
vks::Instance::GetVkInstance()
{
    return mImpl->mInstance;
}
