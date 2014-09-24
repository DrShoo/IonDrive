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

            const int GetVideoModeWidth()
            {
                return _videoMode->ListAt(0)->IntegerGet();
            }

            const int GetVideoModeHeight()
            {
                return _videoMode->ListAt(1)->IntegerGet();
            }

            const int GetVideoModeDepth()
            {
                return _videoMode->ListAt(2)->IntegerGet();
            }

            const bool GetVideoModeWindowed()
            {
                return (_videoMode->ListAt(3)->SymbolGetId() == L"windowed");
            }

        }

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
}