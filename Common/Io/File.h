#pragma once

COMMON_IO_FILE_NS_BEGIN

typedef void* FileHandle;

typedef std::function<void(const FileHandle, const ErrorCode)> OpenCallback;
ErrorCode Open(const std::wstring &path, const OpenCallback &callback);

void Close(const FileHandle file);

bool IsValidHandle(const FileHandle file);

std::wstring ReadAllSync(const boost::filesystem::path &path);

COMMON_IO_FILE_NS_END