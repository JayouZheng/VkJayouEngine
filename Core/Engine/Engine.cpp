/*********************************************************************
 *  Engine.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "Engine.h"
#include "Core/Base/ResourcePool.h"
#include "Core/Base/BaseLayer.h"
#include "Core/Platform/Windows/Window.h"
#include "Core/Scene/Scene.h"
#include <mutex>

namespace
{
    Engine* g_engine = nullptr;

    std::mutex g_enRead;
    std::mutex g_enWrite;

    struct InternalData
    {
        // Global resource pointer.
        BaseLayer*            BaseLayerRef = nullptr;
        Window*               WindowRef    = nullptr;
        Scene*                SceneRef     = nullptr;
        TimerUtil::GameTimer* TimerRef     = nullptr;

        Engine::ModuleInfo    ModuleInfo;

        bool bIsEngineInitialized = false;
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
    if (g_data.TimerRef != nullptr)
    {
        delete g_data.TimerRef;
        g_data.TimerRef = nullptr;
    }
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
    g_data.BaseLayerRef->PreInit();

    ResourcePool::Get()->Push(g_data.BaseLayerRef);

    g_data.WindowRef = Window::Create(g_data.BaseLayerRef);
    g_data.WindowRef->Init();

    g_data.BaseLayerRef->PostInit();

    g_data.SceneRef = Scene::Create(g_data.BaseLayerRef);
    g_data.SceneRef->Init();

    g_data.TimerRef = new TimerUtil::GameTimer;
  
    g_data.ModuleInfo = { _str_null, _str_null };

    g_data.bIsEngineInitialized = true;
}

void Engine::Run()
{
    if (!g_data.bIsEngineInitialized)
    {
        _log_error("The Engine is uninitialized!", LogSystem::Category::Engine);
        RequireExit(1);
    }

    g_data.WindowRef->Show();
}

void Engine::Tick()
{
    // Engine Loop.
    if (!g_data.bIsEngineInitialized)
    {
        _log_error("The Engine is uninitialized!", LogSystem::Category::Engine);
        RequireExit(1);
    }

    g_data.TimerRef->Tick([&]()
    {
        g_data.SceneRef->Update(g_data.TimerRef);
        g_data.SceneRef->Render(g_data.TimerRef);
    });
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

Window* Engine::GetWindow()
{
    return g_data.WindowRef;
}

Scene* Engine::GetScene()
{
    return g_data.SceneRef;
}

string Engine::GetModulePath() const
{
    return g_data.ModuleInfo.Path;
}

string Engine::GetModuleName() const
{
    return g_data.ModuleInfo.Name;
}

WindowDesc Engine::GetWindowDesc() const
{
    if (g_data.WindowRef != nullptr)
    {
        return g_data.WindowRef->GetWindowDesc();
    }
    else
    {
        return WindowDesc();
    }
}
