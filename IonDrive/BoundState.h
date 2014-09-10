#pragma once

namespace iondrive
{
    namespace bound
    {
        const std::wstring &GetLogVerbosity();
    }

    bool BindState(ionscript::ILibraryContext &c);

}