#pragma once
#include "engine/graphics/RWorld.h"
#include "engine/master.h"

#include "engine/graphics/Window.h"
#include "engine/graphics/Renderer.h"
#include <vector>
#include <cstdint>
#include "engine/weak_vector.h"
#include "STDGLModel.h"
#include "STDGLWindow.h"
#include "STDGLShaderSystem.h"

/*!
*   \brief An OpenGL renderer.
*/
class STDGLRenderer : public Renderer {
protected:
    GLFWwindow* rendererData = nullptr;
    weak_vector<RWorld> RWorldVec;
    weak_vector<STDGLWindow> WindowVector = weak_vector<STDGLWindow>();
    STDGLModelSystem ModelSystem;
    STDGLShaderSystem ShaderSystem;

    std::array<GLsync, 2> DoubleBufferFences = { nullptr, nullptr };

    GLuint ModelInstanceBlankerShader;
    GLuint ModelInstancePreprocessShader;
    GLuint ModelInstanceReplicatorShader;

    uint64_t FrameCounter = 0;

    void Init();
public:
    ~STDGLRenderer();
    static std::shared_ptr<Renderer> Make();
    std::shared_ptr<RWorld> MakeRWorld();
    Camera* GetCamera(std::string name);
    const uint64_t& GetFrameCounter();
    void Draw();
    std::shared_ptr<Window> MakeWindow(int x, int y, std::string name);
};
