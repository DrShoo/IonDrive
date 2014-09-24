#pragma once

#include "../Defs.h"

IONENGINE_HTML_NS_BEGIN

class RenderHandler;

class BrowserClient : public CefClient
{
public:
    BrowserClient(RenderHandler *renderHandler);

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

private:
    CefRefPtr<CefRenderHandler> _renderHandler;

    IMPLEMENT_REFCOUNTING(BrowserClient);
};

IONENGINE_HTML_NS_END