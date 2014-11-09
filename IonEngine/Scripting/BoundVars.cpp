#include "stdafx.h"

#include "BoundVars.h"

using namespace ionscript;
namespace logger = common::logger;

namespace
{

    ExpressionSptr _logVerbosity = Expression::Symbol(L"trace");

    ExpressionSptr _videoMode = Expression::List();

}

namespace ionengine
{
    namespace scripting
    {
        namespace boundstate
        {

           

        }

        bool BindState(ionscript::ILibraryContext &c)
        {
            logger::Info(L"binding IonDrive state to script...");

            auto success = true;

            success = success &&
                c.LibraryBindNativeVariable(L"log-verbosity", _logVerbosity, [](ExpressionSptr){
            });

            return success;
        }
    }
}