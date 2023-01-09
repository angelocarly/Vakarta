//
// Created by Angelo Carly on 09/01/2023.
//

#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Instance.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/VulkanSession.h"

class vks::VulkanSession::Impl
{
    public:
        Impl();
        ~Impl();

    public:
        vks::Instance & mInstance;
        vks::PhysicalDevicePtr mPhysicalDevice;
        vks::DevicePtr mDevice;
};

vks::DevicePtr GetDevice();

vks::VulkanSession::Impl::Impl()
:
    mInstance( vks::Instance::GetInstance() ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( std::make_shared< vks::Device >( mPhysicalDevice ) )
{

}

vks::VulkanSession::Impl::~Impl()
{
}

// =====================================================================================================================

vks::VulkanSession::VulkanSession()
:
    mImpl( new Impl() )
{

}

vks::VulkanSession::~VulkanSession()
{

}

vks::VulkanSessionPtr
vks::VulkanSession::GetInstance()
{
    static std::shared_ptr< vks::VulkanSession > vksSession = std::shared_ptr< vks::VulkanSession >( new vks::VulkanSession() );
    return vksSession;
}

vks::DevicePtr
vks::VulkanSession::GetDevice()
{
    return mImpl->mDevice;
}
