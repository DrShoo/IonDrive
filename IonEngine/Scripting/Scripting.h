#pragma once

namespace ionengine
{
    namespace scripting
    {

        ionscript::ExpressionSptr EvaluateFile(const std::wstring &path);

        bool Initialize();

        void Shutdown();

    }
}