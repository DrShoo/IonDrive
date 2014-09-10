#pragma once

namespace ionscript
{
    typedef std::shared_ptr<struct CallableSignature> CallableSignatureSptr;
    typedef std::shared_ptr<class Expression> ExpressionSptr;
    typedef std::shared_ptr<class Expression> ExpressionSptr;

    namespace stdlib
    {

        CallableSignatureSptr SetQ_Signature();

        ExpressionSptr SetQ(struct ICallContext &context, ExpressionSptr params);

    }
}