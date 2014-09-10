#include "stdafx.h"

#include "../CallableSignature.h"
#include "../Lexer.h"
#include "../Parser.h"
#include "../VirtualMachine.h"

#include "Init.h"
#include "Common.h"

namespace ionscript
{
    namespace logger = common::log;

    namespace stdlib
    {

        bool Init(ILibraryContext &c)
        {

            logger::Info(L"initializing IonScript stdlib...");

            c.LibraryBindNativeMacro(L"setq", SetQ, SetQ_Signature());

            logger::Info(L"IonScript stdlib was initialized successfully");

            return true;
        }

    }

}