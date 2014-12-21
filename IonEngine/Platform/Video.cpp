#include "stdafx.h"

#include "Platform.h"
#include "Video.h"

namespace logger = common::logger;

namespace
{

    SDL_Window *_mainWnd = nullptr;

    SDL_GLContext _mainGlContext = nullptr;

}

IONENGINE_PLATFORM_VIDEO_NS_BEGIN

bool Initialize(const int w, const int h, const bool fullscreen)
{
    logger::Info(L"setting up preliminary OpenGL params...");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    logger::Info(L"creating main SDL window...");

    auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    else
    {
        flags |= SDL_WINDOW_BORDERLESS;
    }

    auto header = boost::format("IonDrive %s");
    header % platform::GetProductVersion();

    _mainWnd = SDL_CreateWindow(
        header.str().c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w,
        h,
        flags);
    if (!_mainWnd)
    {
        logger::Fatal(L"main SDL window creation failed");
        return false;
    }

    logger::Info(L"initializing OpenGL context...");

    _mainGlContext = SDL_GL_CreateContext(_mainWnd);
    if (!_mainGlContext)
    {
        logger::Fatal(L"OpenGL context creation failed");
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(-1, 0, 0);
    glRasterPos2d(0, 1);
    glPixelZoom(1, -1);

    return true;
}

void Shutdown()
{
    logger::Info(L"destroying opengl context (0x%p)", _mainGlContext);
    if (_mainGlContext)
    {
        SDL_GL_DeleteContext(_mainGlContext);
        _mainGlContext = nullptr;
    }

    logger::Info(L"destroying main window (0x%p)", _mainWnd);
    if (_mainWnd)
    {
        SDL_DestroyWindow(_mainWnd);
        _mainWnd = nullptr;
    }
}

IONENGINE_PLATFORM_VIDEO_NS_END