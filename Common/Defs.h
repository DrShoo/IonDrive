#pragma once

#if !defined(OUT)
    #define OUT
#endif

#if !defined(INOUT)
    #define INOUT
#endif

#if !defined(UNUSED_PARAMETER)
    #define UNUSED_PARAMETER(X) (X)
#endif

#define COMMON_PARALLEL_NS_BEGIN namespace common { namespace parallel {
#define COMMON_PARALLEL_NS_END }}

#define COMMON_IO_FILE_NS_BEGIN namespace common { namespace io { namespace file {
#define COMMON_IO_FILE_NS_END }}}

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

    enum class ErrorCode
    {
        Invalid,
        Min,

        Ok,
        GeneralError,

        ParallelTimeout,

        FileNotFound,
        FileBusy,

        Max
    };

}