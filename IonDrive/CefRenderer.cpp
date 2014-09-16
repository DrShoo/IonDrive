#include "stdafx.h"

#include "CefRenderer.h"

namespace iondrive
{ 
    bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
    {
        browser;
        rect;

        return false;
    }

    void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {
        browser;
        type;
        dirtyRects;
        buffer;
        width;
        height;
    }

    BrowserClient::BrowserClient(RenderHandler *renderHandler)
    {
        _renderHandler = renderHandler;
    }

    CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler()
    {
        return _renderHandler;
    }
}