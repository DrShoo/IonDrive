#pragma once

namespace iondrive
{
    namespace bound
    {
        const int GetVideoModeWidth();

        const int GetVideoModeHeight();

        const int GetVideoModeDepth();

        const bool GetVideoModeWindowed();
    }

    bool BindState(ionscript::ILibraryContext &c);

}