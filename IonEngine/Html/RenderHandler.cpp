#include "stdafx.h"

#include "RenderHandler.h"

IONENGINE_HTML_NS_BEGIN

RenderHandler::RenderHandler()
{
    _wnd = nullptr;
}

bool RenderHandler::Init(SDL_Window *wnd)
{
    assert(wnd);
    assert(!_wnd);

    _wnd = wnd;

    return true;
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    browser;

    int w = 0;
    int h = 0;
    SDL_GetWindowSize(_wnd, &w, &h);

    rect = CefRect(0, 0, w, h);
    return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{
    browser;
    type;
    dirtyRects;
    buffer;
    width;
    height;

    assert(_wnd);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    SDL_GL_SwapWindow(_wnd);
}

IONENGINE_HTML_NS_END