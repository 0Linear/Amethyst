#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

namespace Filesystem {
    std::ifstream GetFile(const std::string& name, const std::ios_base::openmode& flags);
}