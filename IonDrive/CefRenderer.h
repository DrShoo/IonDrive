#pragma once

namespace iondrive
{
    class RenderHandler : public CefRenderHandler
    {
    public:
        

    private:
        virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

        virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override;

        IMPLEMENT_REFCOUNTING(RenderHandler);
    };
}