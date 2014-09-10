#include "stdafx.h"

#include "File.h"

#include "utf8_codecvt_facet.cpp"

namespace fs = boost::filesystem;
namespace fsd = boost::filesystem::detail;

namespace
{
    const std::locale _utf8Locale(std::locale(), new fsd::utf8_codecvt_facet());
}

namespace common
{
    namespace file
    {
        std::wstring ReadAll(const fs::path &path)
        {
            assert(exists(path));

            std::wifstream wifs(path.c_str());
            wifs.imbue(_utf8Locale);

            const auto size = boost::numeric_cast<size_t>(file_size(path));
            std::vector<wchar_t> text(size + 1, L'\0');
            wifs.read(&text[0], size);

            return &text[0];
        }
    }
}