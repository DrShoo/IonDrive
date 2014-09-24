#include "stdafx.h"

#include "BrowserClient.h"
#include "RenderHandler.h"

#include "../Defs.h"

IONENGINE_HTML_NS_BEGIN

BrowserClient::BrowserClient(RenderHandler *renderHandler)
{
    _renderHandler = renderHandler;
}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler()
{
    assert(_renderHandler);
    return _renderHandler;
}

IONENGINE_HTML_NS_END