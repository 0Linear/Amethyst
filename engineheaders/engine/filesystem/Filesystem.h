#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

namespace Filesystem {
    std::ifstream GetFileAsStream(const std::string& name, const std::ios_base::openmode& flags);
    std::ofstream GetFileOutputStream(const std::string& name, const std::ios_base::openmode& flags);
}