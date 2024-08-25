// openglwindow.hpp

#pragma once

#include <functional>

#include "../../../third-party/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

using namespace std;

using RenderCallback = function<void (GLFWwindow *window)>;

class OpenGLWindow {
    public:
        OpenGLWindow(int width,
                     int height,
                     bool isFixed,
                     const char *title);

        ~OpenGLWindow();

        void LoadOpenGLFunctions();
        void RenderLoop(RenderCallback renderCallback);

        int GetWindowWidth() const;
        int GetWindowHeight() const;

    private:
        GLFWwindow *glfwWindow;
        static int windowWidth, windowHeight;

        static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);

        void CreateWindow(int width,
                          int height,
                          const char *title);
};
