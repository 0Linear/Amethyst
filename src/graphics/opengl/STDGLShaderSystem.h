#pragma once

#include <glad/glad.h>
#include "engine/filesystem/Filesystem.h"
#include <unordered_map>
#include "engine/filesystem/ADF.h"

class STDGLShaderSystem {
    std::unordered_map<std::string, GLuint> ComputeShaders;
    void InitCompute(const ADFEntry& Shaderdefs);
public:
    void Init();

    inline GLuint GetComputeShader(std::string name) {
        return ComputeShaders.at(name);
    }
};