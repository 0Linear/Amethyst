#include "STDGLShaderSystem.h"
#include "Shadinclude.hpp"
#include "engine/master.h"

void STDGLShaderSystem::InitCompute(const ADFEntry& Shaderdefs) {
    const auto& ShaderDefList = Shaderdefs.GetChildren();
    if (ShaderDefList.empty()) return;

    for (const auto& shader : ShaderDefList) {
        if (!shader.second.HasChild("Source")) {
            Engine::Warning("Shader " + shader.first + " does not have a \"Source\", ignoring!");
            continue;
        }
        auto Shaderpath = shader.second["Source"];
        std::string ShaderSrc = Shadinclude::load("scripts/shaders/opengl/" + Shaderpath.GetString()); 
        
        GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
        auto ShaderSrc_Cstr = ShaderSrc.c_str();
        glShaderSource(computeShader, 1, &ShaderSrc_Cstr, NULL);

        glCompileShader(computeShader);

        int success;
        char infoLog[512];
        glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            Engine::Warning("Compute shader compilation error! Log printed to std::cout");
            glDeleteShader(computeShader);
            continue;
        }

        GLuint computeProgram = glCreateProgram();
        glAttachShader(computeProgram, computeShader);
        glLinkProgram(computeProgram);

        glGetProgramiv(computeProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(computeProgram, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            Engine::Warning("Compute shader program linking error! Log printed to std::cout");
            glDeleteProgram(computeProgram);
        }

        ComputeShaders.emplace(shader.first, computeProgram);

    }
}


void STDGLShaderSystem::Init() {
    auto glshadersadf = ADFEntry::FromFile("scripts/shaders/glshaders.adf")["Shaders"];

    auto ComputeShaderDefs = glshadersadf["Compute"];
    InitCompute(ComputeShaderDefs);
}