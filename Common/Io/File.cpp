#include "stdafx.h"

#include "../Defs.h"
#include "utf8_codecvt_facet.cpp"

#include "File.h"

namespace fs = boost::filesystem;
namespace fsd = boost::filesystem::detail;

namespace
{
    using namespace common::io::file;

    std::map<FileHandle, int> _filesTable;

    const std::locale _utf8Locale(std::locale(), new fsd::utf8_codecvt_facet());
}

COMMON_IO_FILE_NS_BEGIN

ErrorCode Open(const std::wstring &path, const OpenCallback &callback)
{
    UNUSED_PARAMETER(path);
    UNUSED_PARAMETER(callback);



    return ErrorCode::Ok;
}

void Close(FileHandle file)
{
    UNUSED_PARAMETER(file);
}

bool IsValidHandle(FileHandle file)
{
    return (_filesTable.count(file) > 0);
}

std::wstring ReadAllSync(const fs::path &path)
{
    assert(exists(path));

    std::wifstream wifs(path.c_str());
    wifs.imbue(_utf8Locale);

    const auto size = boost::numeric_cast<size_t>(file_size(path));
    std::vector<wchar_t> text(size + 1, L'\0');
    wifs.read(&text[0], size);

    return &text[0];
}

COMMON_IO_FILE_NS_END