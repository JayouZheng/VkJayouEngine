//
// ResourcePool.h
//

#pragma once

#include "Core/SmartPtr/VkSmartPtr.h"

class IResourceHandler;

class ResourcePool
{
private:

    ResourcePool() = default;

public:

    ~ResourcePool();

    static ResourcePool*& Get();

    void Push(IResourceHandler* InRef);
    void Push(VkSmartPtr<VkObjectHandler> InRef);

    void Free();
};