#pragma once

#if !defined(OUT)
    #define OUT
#endif

#if !defined(INOUT)
    #define INOUT
#endif

namespace common
{

    inline bool IsDebug()
    {
#if defined(_DEBUG)
        return true;
#else
        return false;
#endif
    }

}