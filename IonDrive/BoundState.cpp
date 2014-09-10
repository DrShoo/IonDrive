#include "stdafx.h"

#include "BoundState.h"

using namespace ionscript;
namespace logger = common::log;

namespace
{

    ExpressionSptr _logVerbosity = Expression::Symbol(L"trace");

}

namespace iondrive
{


    bool BindState(ionscript::ILibraryContext &c)
    {
        logger::Info(L"binding IonDrive state to script...");

        return c.LibraryBindNativeVariable(L"log-verbosity", _logVerbosity, [](ExpressionSptr){
        });
    }

}