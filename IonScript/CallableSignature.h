#pragma once

namespace ionscript
{

    struct CallableSignature : boost::noncopyable
    {
        CallableSignature(const bool isVariadic);

        struct ArgumentInfo : boost::noncopyable
        {
            ArgumentInfo(const std::wstring &id);

            const std::wstring Id;
        };

        const bool IsVariadic;
        
    };

    typedef std::shared_ptr<CallableSignature> CallableSignatureSptr;

}