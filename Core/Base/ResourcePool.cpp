﻿//
// ResourcePool.cpp
//

#include "ResourcePool.h"
#include "Interface/IResourceHandler.h"
#include <mutex>

namespace
{
    static ResourcePool* g_resourcePool;

    static std::mutex g_respRead;
    static std::mutex g_respWrite;

    struct InternalResource
    {
        std::vector<IResourceHandler*>           CommonRefs;
        std::vector<VkSmartPtr<VkObjectHandler>> VkSmartRefs;

    }g_resource;

    static class ResourcePoolHandler
    {
    public:
        ~ResourcePoolHandler()
        {
            if (g_resourcePool != nullptr)
                ResourcePool::Get()->Free();
        }
    }RespHandler;
}

ResourcePool::~ResourcePool()
{
    for (auto& ref : g_resource.CommonRefs)
        if (ref != nullptr)
        {
            delete ref;
            ref = nullptr;
        }
}

ResourcePool*& ResourcePool::Get()
{
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    if (g_resourcePool == nullptr)
        g_resourcePool = new ResourcePool;

    return g_resourcePool;
}

void ResourcePool::Push(IResourceHandler* InRef)
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    g_resource.CommonRefs.push_back(InRef);
}

void ResourcePool::Push(VkSmartPtr<VkObjectHandler> InRef)
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    g_resource.VkSmartRefs.push_back(InRef);
}

void ResourcePool::Free()
{
    std::unique_lock<std::mutex> lock_r(g_respRead);
    std::unique_lock<std::mutex> lock_w(g_respWrite);

    delete g_resourcePool;
    g_resourcePool = nullptr;

    // Exit...
}