#include "stdafx.h"

#include "version.h"

namespace boundstate = ionengine::scripting::boundstate;
namespace logger = common::logger;
namespace platform = ionengine::platform;
namespace scripting = ionengine::scripting;

namespace
{
    void MainLoop();

    void OnEvent(const SDL_Event &event);

    bool Startup();

    void Shutdown();
}

int CALLBACK WinMain(
    _In_  HINSTANCE hInstance,
    _In_  HINSTANCE hPrevInstance,
    _In_  LPSTR lpCmdLine,
    _In_  int nCmdShow
    )
{
    hInstance;
    hPrevInstance;
    lpCmdLine;
    nCmdShow;

    if (!Startup())
    {
        Shutdown();
        return 1;
    }

    MainLoop();

    Shutdown();
    return 0;
}

namespace
{
    bool _running = true;

    bool InitHtmlEngine()
    {
        return true;
    }

    bool InitScripting()
    {
        if (!scripting::Initialize())
        {
            return false;
        }

        const std::wstring CFG_FILENAME = L"config.ionscript";
        return (bool)scripting::EvaluateFile(CFG_FILENAME);
    }

    void MainLoop()
    {
        logger::Info(L"entering main loop...");

        while (_running)
        {
            SDL_Event event = { 0 };
            while (SDL_PollEvent(&event))
            {
                OnEvent(event);
            }
        }

        logger::Info(L"exiting from main loop");
    }

    void OnEvent(const SDL_Event &event)
    {
        if (event.type == SDL_QUIT)
        {
            logger::Info(L"received SDL_QUIT event, stopping...");
            _running = false;
        }
    }

    bool Startup()
    {
        logger::Initialize();

        logger::Write(
            L"**********************************************************************\n"
            L"*** IonDrive engine log, build %s\n"
            L"***\n\n",
            VERSION_PRODUCTSTR);

        return platform::Initialize(VERSION_PRODUCTSTR) &&
               InitScripting() &&
               InitHtmlEngine();
    }

    void Shutdown()
    {
        scripting::Shutdown();

        platform::Shutdown();

        logger::Info(L"shutdown completed successfully");

        logger::Uninitialize();
    }
}