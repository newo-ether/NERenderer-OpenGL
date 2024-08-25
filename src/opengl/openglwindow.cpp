// opengl.cpp

#include "include/openglwindow.hpp"

#include <iostream>

#include "../../third-party/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

using namespace std;

int OpenGLWindow::windowWidth = 0;
int OpenGLWindow::windowHeight = 0;

OpenGLWindow::OpenGLWindow(int width,
                           int height,
                           bool isFixed,
                           const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (isFixed) {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }

    CreateWindow(width, height, title);

    windowWidth = width;
    windowHeight = height;
}

OpenGLWindow::~OpenGLWindow() {
    glfwTerminate();
}

void OpenGLWindow::CreateWindow(int width,
                                int height,
                                const char *title) {

    glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (glfwWindow == NULL) {
        cout << "[Error] OpenGLWindow: Failed to create GLFW window." << endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(glfwWindow);
    glfwSetFramebufferSizeCallback(glfwWindow, frameBufferSizeCallback);
}

void OpenGLWindow::LoadOpenGLFunctions() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "[Error] OpenGLWindow: Failed to initialize GLAD." << endl;
        glfwTerminate();
        exit(-1);
    }
}

void OpenGLWindow::RenderLoop(RenderCallback renderCallback) {

    while (!glfwWindowShouldClose(glfwWindow))
    {
        renderCallback(glfwWindow);
    }
}

void OpenGLWindow::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);
}

int OpenGLWindow::GetWindowWidth() const {
    return windowWidth;
}

int OpenGLWindow::GetWindowHeight() const {
    return windowHeight;
}
