#pragma once

#include "../Defs.h"

IONENGINE_HTML_NS_BEGIN

class RenderHandler;
class BrowserClient;

class HtmlRenderer
{
public:
    HtmlRenderer();

private:
    CefRefPtr<RenderHandler> _renderHandler;

    CefRefPtr<BrowserClient> _browserClient;

    CefRefPtr<CefBrowser> _browser;
};

IONENGINE_HTML_NS_END