/*********************************************************************
 *  ResourcePool.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "ResourcePool.h"
#include "Interface/IResourceHandler.h"
#include <mutex>

namespace
{
    ResourcePool* g_resourcePool;

    std::mutex g_respRead;
    std::mutex g_respWrite;

    struct InternalResource
    {
        std::vector<IResourceHandler*>           CommonRefs;
        std::vector<VkSmartPtr<VkObjectHandler>> VkSmartRefs;

    }g_resource;

    class ResourcePoolHandler
    {
    public:

        void Free()
        {
            std::unique_lock<std::mutex> lock_r(g_respRead);
            std::unique_lock<std::mutex> lock_w(g_respWrite);

            if (g_resourcePool != nullptr)
            {
                delete g_resourcePool;
                g_resourcePool = nullptr;
            }
        }

        ~ResourcePoolHandler()
        {
            Free();
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
