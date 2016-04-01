#pragma once

#include <string>
#include <vector>

#include "stde_common.h"

STDE_NS_BEGIN

void string_split(const std::string &str, const std::string &delimiter, std::vector<std::string> *strings);

STDE_NS_END