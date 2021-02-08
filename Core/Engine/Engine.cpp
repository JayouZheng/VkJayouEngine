/*********************************************************************
 *  Engine.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Engine.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Base/ResourcePool.h"
#include <mutex>

namespace
{
    Engine* g_engine;

    std::mutex g_enRead;
    std::mutex g_enWrite;

    struct InternalData
    {
        BaseLayer* BaseLayerRef;
        Engine::ModuleInfo ModuleInfo;
    }g_data;

    class EngineHandler
    {
    public:

        void Free()
        {
            std::unique_lock<std::mutex> lock_r(g_enRead);
            std::unique_lock<std::mutex> lock_w(g_enWrite);

            if (g_engine != nullptr)
            {
                delete g_engine;
                g_engine = nullptr;
            }
        }

        ~EngineHandler()
        {
            Free();
        }
    }EngineHandlerInstance;
}

Engine::~Engine()
{
}

Engine*& Engine::Get()
{
    std::unique_lock<std::mutex> lock_w(g_enWrite);

    if (g_engine == nullptr)
        g_engine = new Engine;

    return g_engine;
}

void Engine::Init()
{
    g_data.BaseLayerRef = BaseLayer::Create(nullptr);
    ResourcePool::Get()->Push(g_data.BaseLayerRef);
    g_data.BaseLayerRef->Init();
    g_data.ModuleInfo = { _str_null, _str_null };
}

void Engine::RequireExit(int32 InCode)
{
    _log_common(StringUtil::Printf("Engine::RequireExit with Code %\n ------>Suddenly exit will confuse memory laek check, never mind.", InCode), LogSystem::Category::Engine);
    exit(InCode);
}

void Engine::CacheModuleInfo(const ModuleInfo& InModuleInfo)
{
    static bool bDoOnce = false;
    if (!bDoOnce)
    {
        g_data.ModuleInfo = InModuleInfo;
        bDoOnce = true;
    }
}

BaseLayer* Engine::GetBaseLayer()
{
    return g_data.BaseLayerRef;
}

string Engine::GetModulePath() const
{
    return g_data.ModuleInfo.Path;
}

string Engine::GetModuleName() const
{
    return g_data.ModuleInfo.Name;
}
