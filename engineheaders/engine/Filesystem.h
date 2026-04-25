#pragma once

#include <fstream>
#include <sstream>
#include "vdfparser/vdfparser.hpp"
#include <iostream>

namespace Filesystem {
    std::ifstream GetFile(const std::string& name, const std::ios_base::openmode& flags);
    VdfParser::KeyValue GetDataFile(const std::string& name);
}