#include "stdafx.h"

#include "Scripting.h"
#include "BoundVars.h"

namespace logger = common::logger;

namespace
{
    ionscript::VirtualMachineSptr _vm;
}

namespace ionengine
{
    namespace scripting
    {

        ionscript::ExpressionSptr EvaluateFile(const std::wstring &path)
        {
            logger::Info(L"evaluating ionscript file '%s'...", path);

            const auto text = common::io::file::ReadAllSync(path);

            auto expr = _vm->EvaluateFile(path, text);
            if (!expr)
            {
                logger::Fatal(L"evaluating ionscript file '%s' was failed", path);
                for (const auto &error : _vm->GetParserErrors())
                {
                    logger::Error(L"\n%s", error->FormatedMessage());
                }

                for (const auto &error : _vm->GetRuntimeErrors())
                {
                    logger::Error(L"\n%s", error->FormatedMessage());
                }
            }

            return expr;
        }

        bool Initialize()
        {
            logger::Info(L"initializing IonScript engine...");

            assert(!_vm);
            _vm = ionscript::VirtualMachine::Create();
            _vm->Init();

            if (!BindState(*_vm))
            {
                return false;
            }

            return true;
        }

        void Shutdown()
        {
            logger::Info(L"destroying IonScript engine");

            _vm.reset();
        }

    }
}