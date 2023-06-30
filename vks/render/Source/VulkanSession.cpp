//
// Created by Angelo Carly on 09/01/2023.
//

#include "vks/render/Device.h"
#include "vks/render/ForwardDecl.h"
#include "vks/render/Instance.h"
#include "vks/render/PhysicalDevice.h"
#include "vks/render/VksSession.h"

class vks::VksSession::Impl
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

vks::VksSession::Impl::Impl()
:
    mInstance( vks::Instance::GetInstance() ),
    mPhysicalDevice( std::make_shared< vks::PhysicalDevice >( mInstance ) ),
    mDevice( mInstance.CreateDevice( mPhysicalDevice ) )
{

}

vks::VksSession::Impl::~Impl()
{
    mDevice.reset();
}

// =====================================================================================================================

vks::VksSession::VksSession()
:
    mImpl( new Impl() )
{

}

vks::VksSession::~VksSession()
{

}

vks::VksSessionPtr
vks::VksSession::GetInstance()
{
    static std::shared_ptr< vks::VksSession > vksSession = std::shared_ptr< vks::VksSession >( new vks::VksSession() );
    return vksSession;
}

vks::DevicePtr
vks::VksSession::GetDevice()
{
    return mImpl->mDevice;
}
