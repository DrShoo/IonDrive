#pragma once

#include "../Defs.h"

IONENGINE_HTML_NS_BEGIN

class DocumentHost final
{
public:
    DocumentHost();
    ~DocumentHost();

    bool Init(const int x, const int y, const int w, const int h);

private:
    HWINDOW _wnd;

};

IONENGINE_HTML_NS_END