#include "STDGLShaderSystem.h"
#include "Shadinclude.hpp"
#include "engine/master.h"

void STDGLShaderSystem::InitCompute(VdfParser::KeyValue Shaderdefs) {
    if (!Shaderdefs.getChildren().has_value()) return;

    auto ShaderDefList = Shaderdefs.getChildren().value();
    for (const auto& shader : ShaderDefList) {
        auto Shaderpath = shader.second.getChild("Source");
        if (!Shaderpath.has_value()) {
            std::cout << "Shader " + shader.first + " does not have a \"Source\", ignoring!" << std::endl;
            continue;
        }
        std::string ShaderSrc = Shadinclude::load("scripts/shaders/opengl/" + std::get<std::string>(Shaderpath->value)); 
        
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
    auto glshadersadf = Filesystem::GetDataFile("scripts/shaders/glshaders.adf").getChild("Shaders").value();

    auto ComputeShaderDefs = glshadersadf.getChild("Compute");
    if (ComputeShaderDefs.has_value()) {
        InitCompute(ComputeShaderDefs.value());
    }
}