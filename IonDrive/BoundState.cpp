#include "stdafx.h"

#include "BoundState.h"

using namespace ionscript;
namespace logger = common::log;

namespace
{

    ExpressionSptr _logVerbosity = Expression::Symbol(L"trace");

    ExpressionSptr _videoMode = Expression::List();

}

namespace iondrive
{


    bool BindState(ionscript::ILibraryContext &c)
    {
        logger::Info(L"binding IonDrive state to script...");

        auto success = true;

        success = success && 
            c.LibraryBindNativeVariable(L"log-verbosity", _logVerbosity, [](ExpressionSptr){
            });

        success = success && 
            c.LibraryBindNativeVariable(L"video-mode", _videoMode, [](ExpressionSptr){
            });

        return success;
    }

}