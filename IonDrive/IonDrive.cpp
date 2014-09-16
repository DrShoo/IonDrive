#include "stdafx.h"

#include "version.h"
#include "BoundState.h"

namespace logger = common::log;

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

    SDL_Window *_mainWnd = nullptr;

    ionscript::VirtualMachineSptr _insVm;

    bool InitSdl()
    {
        logger::Info(L"initializing SDL...");
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            logger::Fatal(L"SDL initialization failed");
            return false;
        }

        logger::Info(L"SDL initialization succeed");

        logger::Info(L"creating main SDL window...");

        auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
        if (!iondrive::bound::GetVideoModeWindowed())
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }

        _mainWnd = SDL_CreateWindow(
            "IonDrive "VERSION_PRODUCTSTR, 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            iondrive::bound::GetVideoModeWidth(), 
            iondrive::bound::GetVideoModeHeight(),
            flags);
        if (!_mainWnd)
        {
            logger::Fatal(L"main SDL window creation failed");
            return false;
        }

        return true;
    }

    bool InitIse()
    {
        const std::wstring CFG_FILENAME = L"config.ionscript";

        logger::Info(L"initializing IonScript engine...");

        assert(!_insVm);
        _insVm = ionscript::VirtualMachine::Create();
        _insVm->Init();

        if (!iondrive::BindState(*_insVm))
        {
            return false;
        }

        logger::Info(L"loading basic configuration file '%s'...", CFG_FILENAME);

        const auto text = common::file::ReadAll(CFG_FILENAME);

        auto expr = _insVm->EvaluateFile(CFG_FILENAME, text);
        if (!expr)
        {
            logger::Fatal(L"loading config file '%s' was failed", CFG_FILENAME);
            for (const auto &error : _insVm->GetParserErrors())
            {
                logger::Error(L"\n%s", error->FormatedMessage());
            }

            for (const auto &error : _insVm->GetRuntimeErrors())
            {
                logger::Error(L"\n%s", error->FormatedMessage());
            }

            return false;
        }

        return true;
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

        return InitIse() && InitSdl();
    }

    void Shutdown()
    {
        logger::Info(L"destroying main window (0x%p)", _mainWnd);
        if (_mainWnd)
        {
            SDL_DestroyWindow(_mainWnd);
            _mainWnd = nullptr;
        }

        logger::Info(L"destroying SDL");
        SDL_Quit();

        logger::Info(L"destroying IonScript engine");
        _insVm.reset();

        logger::Info(L"shutdown completed successfully");

        logger::Uninitialize();
    }
}