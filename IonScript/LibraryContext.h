#pragma once

namespace ionscript
{
    typedef std::shared_ptr<class Expression> ExpressionSptr;
    typedef std::shared_ptr<struct CallableSignature> CallableSignatureSptr;

    typedef std::function<ExpressionSptr(struct ICallContext&, ExpressionSptr)> NativeMacroImpl;
    typedef std::function<void(ExpressionSptr)> VariableChangedCallback;

    typedef std::weak_ptr<class Expression> ExpressionWptr;

    struct ILibraryContext
    {
        virtual bool LibraryBindNativeMacro(const std::wstring &id, NativeMacroImpl impl, CallableSignatureSptr signature) = 0;

        virtual bool LibraryBindNativeVariable(const std::wstring &id, ExpressionSptr value, VariableChangedCallback callback) = 0;
    };

}