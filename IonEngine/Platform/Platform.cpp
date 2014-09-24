#include "stdafx.h"

#include "Platform.h"

namespace logger = common::logger;

namespace
{
    std::string _productVersion;
}

IONENGINE_PLATFORM_NS_BEGIN

const std::string &GetProductVersion()
{
    assert(!_productVersion.empty());
    return _productVersion;
}

bool Initialize(const std::string &version)
{
    assert(_productVersion.empty());
    assert(!version.empty());

    logger::Info(L"initializing platform v%s", version.c_str());
    _productVersion = version;

    SDL_version v = { 0 };
    SDL_GetVersion(&v);
    logger::Info(L"initializing SDL v%d.%d.%d...", v.major, v.minor, v.patch);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        logger::Fatal(L"SDL initialization failed");
        return false;
    }

    return true;
}

void Shutdown()
{
    logger::Info(L"destroying SDL");

    SDL_Quit();
}

IONENGINE_PLATFORM_NS_END