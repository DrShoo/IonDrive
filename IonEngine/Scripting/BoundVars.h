#pragma once

namespace ionengine
{
    namespace scripting
    {
        namespace boundstate
        {
            const std::wstring &GetLogVerbocity();
        }

        bool BindState(ionscript::ILibraryContext &c);
    }
}