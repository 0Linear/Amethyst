#pragma once

#include "engine/graphics/Window.h"
#include <GLFW/glfw3.h>
#include "imgui.h"

class STDGLWindow : public Window {
protected:
    std::shared_ptr<Renderer> rendererRef = nullptr;
    GLFWwindow* rendererData = nullptr;
    ImGuiContext* UIData = nullptr;
    GLFWwindow* data = nullptr;
    bool ShouldEatCursor = false;
    int Width;
    int Height;
    void ProcessCursorEating();
    void Update();

    friend class GLMisc;
public:
    ~STDGLWindow();
    STDGLWindow(std::weak_ptr<Renderer> RendererWeakPtr, GLFWwindow* RendererDataPtr, int ResX, int ResY, std::string name);

    void SetEatCursor(bool state);
    bool IsEatingCursor();
    bool IsWindowInFocus();
    void Draw();
    void SetName(std::string name);
    void SetResolution(int x, int y);
};