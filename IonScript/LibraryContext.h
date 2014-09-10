#pragma once

namespace ionscript
{
    typedef std::shared_ptr<class Expression> ExpressionSptr;
    typedef std::shared_ptr<struct CallableSignature> CallableSignatureSptr;
    typedef std::function<ExpressionSptr(struct ICallContext&, ExpressionSptr)> NativeMacroImpl;

    struct ILibraryContext
    {
        virtual bool LibraryBindNativeMacro(const std::wstring &id, NativeMacroImpl impl, CallableSignatureSptr signature) = 0;
    };

}