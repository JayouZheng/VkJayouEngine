//
// ResourcePool.cpp
//

#include "ResourcePool.h"
#include <mutex>

namespace
{
    static ResourcePool* g_resourcePool;

    static std::mutex g_respRead;
    static std::mutex g_respWrite;

    struct InternalResource
    {
        std::vector<std::shared_ptr<IResourceHandler>> SharedRef;
        std::vector<SmartPtr<IResourceHandler>>        SmartRef;
        std::vector<VkSmartPtr<VkObjectHandler>>       VkSmartRef;

    }g_resource;
}

ResourcePool*& ResourcePool::Get()
{
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    if (g_resourcePool == nullptr)
        g_resourcePool = new ResourcePool;

    return g_resourcePool;
}

void ResourcePool::Push(std::shared_ptr<IResourceHandler> InRef)
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    g_resource.SharedRef.push_back(InRef);
}

void ResourcePool::Push(SmartPtr<IResourceHandler> InRef)
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    g_resource.SmartRef.push_back(InRef);
}

void ResourcePool::Push(VkSmartPtr<VkObjectHandler> InRef)
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    g_resource.VkSmartRef.push_back(InRef);
}

void ResourcePool::Free()
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    delete this;

    // Exit...
}