#pragma once

#include "../Defs.h"

IONENGINE_HTML_NS_BEGIN

class RenderHandler : public CefRenderHandler
{
public:
    RenderHandler();

    bool Init(SDL_Window *wnd);

private:
    SDL_Window *_wnd;

    virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

    IMPLEMENT_REFCOUNTING(RenderHandler);
};

IONENGINE_HTML_NS_END

