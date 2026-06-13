#pragma once

#define STDGLMODEL_INSTANCE_MAX_COUNT 4096
#define STDGLMODEL_LOD_MAX_COUNT 4
#define STDGLMODEL_MESH_MAX_COUNT 8
#define STDGLMODEL_INSTANCE_PREPROCESS_GROUP_SIZE 256

#include <glad/glad.h>
#include "STDGLCamera.h"
#include "engine/graphics/ModelInstance.h"
#include "engine/geometry/Model.h"
#include "GLFW/glfw3.h"
#include "IndirectDrawBuffer.h"
#include <cstdint>
#include <memory>
#include <queue>

struct STDGLModel {
    void Draw() {
        glBindVertexArray(VAO);
        for (int LOD = 0; LOD < LODCount; LOD++) {
            for (int mesh = 0; mesh < LODs[LOD].MeshCount; mesh++) {
                glDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 
                    (void*)((sizeof(DrawElementsIndirectCommand) * STDGLMODEL_MESH_MAX_COUNT * LOD)
                    + sizeof(DrawElementsIndirectCommand) * mesh));
            }
        }
    }
    void DrawDepth();
    inline void BindInfo() {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ModelInfo);
    }
    inline void BindIndirectCommands() {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, ModelInfo);
    }

    struct Mesh {
        unsigned int stub;

        Mesh() {};
    };
    struct LOD {
        uint8_t MeshCount;
        std::array<Mesh, STDGLMODEL_MESH_MAX_COUNT> Meshes;
    };
    struct ModelInfo_t {
        std::array<std::array<DrawElementsIndirectCommand, STDGLMODEL_MESH_MAX_COUNT>, STDGLMODEL_LOD_MAX_COUNT> IndirectBuffers; 
        float Radius = 0.0f;
        std::array<float, STDGLMODEL_LOD_MAX_COUNT> LODDistances;
        std::array<std::array<GLuint, STDGLMODEL_INSTANCE_MAX_COUNT>, STDGLMODEL_LOD_MAX_COUNT> InstanceIndices;
    };

    STDGLModel(std::string path = "error.adf");
    ~STDGLModel();

    uint8_t LODCount;
    std::array<LOD, STDGLMODEL_LOD_MAX_COUNT> LODs;
    std::string Path;
    GLuint VAO;
    GLuint VBO, EBO, ModelInfo;
};

struct STDGLModelInstanceArray {
    struct InstanceArrayBuffer {
        std::array<mat4, STDGLMODEL_INSTANCE_MAX_COUNT> InstanceMatrices;
    };

    GLFWwindow* rendererData;
    std::queue<uint16_t> FreedIndices;
    std::shared_ptr<STDGLModel> Model;
    std::weak_ptr<STDGLModelInstanceArray> selfRef;
    InstanceArrayBuffer* InstanceBufferMapped;
    GLuint InstanceBuffer = 0;
    uint16_t NextIndex = 0;
    bool wasModified = false;
    bool ShouldUseOtherBuffer = false;
    

    STDGLModelInstanceArray(GLFWwindow* data, std::shared_ptr<STDGLModel> model);

    ~STDGLModelInstanceArray();
        
    std::unique_ptr<ModelInstance> MakeModelInstance();
    inline void Bind() {
        if (wasModified) {
            wasModified = false;
            ShouldUseOtherBuffer = !ShouldUseOtherBuffer;
            glFlushMappedNamedBufferRange(InstanceBuffer, 
                sizeof(InstanceArrayBuffer) * ShouldUseOtherBuffer,
                NextIndex * sizeof(mat4));
        }
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, InstanceBuffer,
                        sizeof(InstanceArrayBuffer) * ShouldUseOtherBuffer,
                        sizeof(InstanceArrayBuffer));
    }

};

struct STDGLModelInstance : public ModelInstance {
    std::shared_ptr<STDGLModelInstanceArray> parent;
    uint16_t index;

    void SetMatrix(mat4 Matrix);

    ~STDGLModelInstance();

    STDGLModelInstance(uint16_t Index, std::shared_ptr<STDGLModelInstanceArray> Parent) { index = Index; parent = Parent; }
};


class STDGLModelSystem {
    weak_vector<STDGLModel> Models;
public:
    std::shared_ptr<STDGLModel> GetModel(std::string path);
};