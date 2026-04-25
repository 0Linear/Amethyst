#include "engine/Filesystem.h"
#include "engine/master.h"

std::ifstream Filesystem::GetFile(const std::string& name, const std::ios_base::openmode& flags) {
        return std::ifstream(name, flags);
}