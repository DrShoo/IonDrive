#pragma once

namespace ionengine
{
    namespace scripting
    {
        namespace boundstate
        {
            const int GetVideoModeWidth();

            const int GetVideoModeHeight();

            const int GetVideoModeDepth();

            const bool GetVideoModeWindowed();
        }

        bool BindState(ionscript::ILibraryContext &c);
    }
}