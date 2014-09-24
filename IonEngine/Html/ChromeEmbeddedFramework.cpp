#include "stdafx.h"

#include "ChomeEmbeddedFramework.h"

namespace logger = common::logger;

IONENGINE_HTML_NS_BEGIN

bool Initialize()
{
    logger::Info(L"initializing CEF...");

    CefMainArgs args(::GetModuleHandle(nullptr));
    CefSettings settings;
    settings.no_sandbox = true;
    settings.single_process = true;

    const auto success = CefInitialize(args, settings, nullptr, nullptr);
    if (!success)
    {
        logger::Fatal(L"CEF initialization failed");
        return false;
    }

    return true;
}

void MessageLoop()
{
    CefDoMessageLoopWork();
}

void Shutdown()
{
    logger::Info(L"shutting down CEF");
    CefShutdown();
}

IONENGINE_HTML_NS_END