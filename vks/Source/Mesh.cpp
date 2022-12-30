//
// Created by Angelo Carly on 27/12/2022.
//

#include "vks/Device.h"
#include "vks/ForwardDecl.h"
#include "vks/Mesh.h"
#include "vks/Vertex.h"

vks::Mesh::Mesh()
{

}

vks::Mesh::Mesh( vks::DevicePtr inDevice, std::vector< Vertex > inVertices, std::vector< uint32_t > inIndices )
:
    mDevice( inDevice )
{
    mVertexCount = inVertices.size();
    mIndexCount = inIndices.size();

    vma::AllocationCreateInfo theVertexAllocationInfo;
    theVertexAllocationInfo.usage = vma::MemoryUsage::eAuto;
    theVertexAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    mVertexBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo
        (
            vk::BufferCreateFlags(),
            sizeof( vks::Vertex ) * inVertices.size(),
            vk::BufferUsageFlagBits::eVertexBuffer
        ),
        theVertexAllocationInfo
    );

    void * theVertexData = mDevice->MapMemory( mVertexBuffer );
    std::memcpy( theVertexData, inVertices.data(), sizeof( vks::Vertex ) * inVertices.size() );
    mDevice->UnmapMemory( mVertexBuffer );

    vma::AllocationCreateInfo theIndexAllocationInfo;
    theIndexAllocationInfo.usage = vma::MemoryUsage::eAuto;
    theIndexAllocationInfo.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    mIndexBuffer = mDevice->CreateBuffer
    (
        vk::BufferCreateInfo
        (
            vk::BufferCreateFlags(),
            sizeof( uint32_t ) * inIndices.size(),
            vk::BufferUsageFlagBits::eIndexBuffer
        ),
        theIndexAllocationInfo
    );

    void * theIndexData = mDevice->MapMemory( mIndexBuffer );
    std::memcpy( theIndexData, inIndices.data(), sizeof( std::uint32_t ) * inIndices.size() );
    mDevice->UnmapMemory( mIndexBuffer );
}

vks::Mesh::~Mesh()
{
    if( mDevice )
    {
        mDevice->DestroyBuffer( mVertexBuffer );
        mDevice->DestroyBuffer( mIndexBuffer );
    }
}

vks::Buffer
vks::Mesh::GetVertexBuffer()
{
    return mVertexBuffer;
}

vks::Buffer
vks::Mesh::GetIndexBuffer()
{
    return mIndexBuffer;
}

uint32_t vks::Mesh::GetVertexCount()
{
    return mVertexCount;
}

uint32_t vks::Mesh::GetIndexCount()
{
    return mIndexCount;
}

void vks::Mesh::Draw( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindVertexBuffers( 0, { mVertexBuffer.GetVkBuffer() }, { 0 } );
    inCommandBuffer.bindIndexBuffer( mIndexBuffer.GetVkBuffer(), 0, vk::IndexType::eUint32 );
    inCommandBuffer.drawIndexed( GetIndexCount(), 1, 0, 0, 0 );
}

