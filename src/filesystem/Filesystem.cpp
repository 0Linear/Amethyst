#include "engine/Filesystem.h"
#include "engine/master.h"

std::ifstream Filesystem::GetFile(const std::string& name, const std::ios_base::openmode& flags) {
        return std::ifstream(name, flags);
}

VdfParser::KeyValue Filesystem::GetDataFile(const std::string& name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        Engine::Error("Error opening file: " + name);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return VdfParser::fromString(buffer.str());
}