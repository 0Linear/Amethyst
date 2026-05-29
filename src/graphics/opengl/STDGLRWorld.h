#pragma once

#include "STDGLCamera.h"
#include "engine/graphics/ModelInstance.h"
#include "engine/graphics/RWorld.h"
#include "STDGLRenderer.h"
#include <memory>
#include <vector>
#include "STDGLModel.h"

struct STDGLRWorld : public RWorld {
    std::shared_ptr<Camera> MakeCamera(vec2 resolution, const std::string& name, vec3 position = vec3(0.0f, 0.0f, 0.0f), float yaw = CAMERA_DEFAULT_YAW, float pitch = CAMERA_DEFAULT_PITCH);
    virtual Camera* GetCamera(const std::string& name);
    std::unique_ptr<ModelInstance> MakeModelInstance(const std::string& path = "error.glb");

    ~STDGLRWorld();
    STDGLRWorld(std::weak_ptr<Renderer> Renderer, GLFWwindow* Context, STDGLModelSystem* ModelSystem) {
        renderer = Renderer.lock();
        context = Context;
        modelsystem = ModelSystem;
    }

    weak_vector<STDGLModelInstanceArray> InstanceArrays;
    weak_vector<STDGLCamera> CameraVec;
    std::shared_ptr<Renderer> renderer;
    GLFWwindow* context;
    STDGLModelSystem* modelsystem;
};