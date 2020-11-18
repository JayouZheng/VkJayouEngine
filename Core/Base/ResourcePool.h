//
// ResourcePool.h
//

#pragma once

#include "Core/Base/Interface/IResourceHandler.h"

class ResourcePool
{
private:

    ResourcePool() = default;

public:

    static ResourcePool*& Get();

    void Push(std::shared_ptr<IResourceHandler> InRef);
    void Push(SmartPtr<IResourceHandler> InRef);
    void Push(VkSmartPtr<VkObjectHandler> InRef);

    void Free();
};