#pragma once

#include "../Defs.h"

IONENGINE_PLATFORM_NS_BEGIN

const std::string &GetProductVersion();

bool Initialize(const std::string &productVersion);

void Shutdown();

IONENGINE_PLATFORM_NS_END