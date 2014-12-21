#include "stdafx.h"

#include "DocumentHost.h"

namespace
{
    BOOL SC_CALLBACK SciterWindowProc(HWINDOW hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LPVOID pParam, LRESULT* pResult)
    {
        BOOL handled = FALSE;
        ::SciterProcND(hwnd, msg, wParam, lParam, &handled);
        return handled;
    }
}

IONENGINE_HTML_NS_BEGIN

DocumentHost::DocumentHost()
{
    _wnd = nullptr;
}

DocumentHost::~DocumentHost()
{
    if (_wnd)
    {
        ::PostMessage(_wnd, WM_CLOSE, 0, 0);
    }
}

bool DocumentHost::Init(const int x, const int y, const int w, const int h)
{
    assert(w > 0);
    assert(h > 0);
    if ((h <= 0) || (w <= 0))
    {
        return false;
    }

    assert(!_wnd);
    if (_wnd)
    {
        return false;
    }

    RECT frame = { x, y, x + w, y + h };
    _wnd = ::SciterCreateWindow(SW_TOOL | SW_RESIZEABLE | SW_ALPHA | SW_POPUP, &frame, SciterWindowProc, nullptr, nullptr);

    return true;
}

IONENGINE_HTML_NS_END