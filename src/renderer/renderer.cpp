// renderer.cpp

#include "include/renderer.hpp"

#include "../base/include/camera.hpp"
#include "../base/include/scene.hpp"

#include "../opengl/include/openglwindow.hpp"
#include "../opengl/include/shader.hpp"
#include "../opengl/include/mesh.hpp"
#include "../opengl/include/framebuffer.hpp"
#include "../opengl/include/texture1D.hpp"
#include "../opengl/include/texture2D.hpp"

#include "../image/include/image.hpp"

#include "../../third-party/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <memory>
#include <random>
#include <string>

#define ArrayLength(_array) ((unsigned int) (sizeof(_array) / sizeof(*_array)))

#define VERTEX_SHADER_PATH "src/shader/vertex.glsl"
#define RENDER_FRAGMENT_SHADER_PATH "src/shader/render.glsl"
#define DISPLAY_FRAGMENT_SHADER_PATH "src/shader/display.glsl"

using namespace std;

const float Renderer::vertices[] = {
     1.0f,  1.0f, 0.0f, // Top Right
     1.0f, -1.0f, 0.0f, // Bottom Right
    -1.0f, -1.0f, 0.0f, // Bottom Left
    -1.0f,  1.0f, 0.0f  // Top Left 
};

const unsigned int Renderer::faces[] = {
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};

Renderer::Renderer(shared_ptr<Scene> scene,
                   shared_ptr<Camera> camera,
                   int maxDepth,
                   float russianRoulete,
                   float gamma,
                   bool accumulate):

                   scene(scene),
                   camera(camera),
                   maxDepth(maxDepth),
                   russianRoulete(russianRoulete),
                   gamma(gamma),
                   accumulate(accumulate) {

    screenWidth = camera->GetScreenWidth();
    screenHeight = camera->GetScreenHeight();
    randomSeed = new unsigned int[screenWidth * screenHeight];
}

Renderer::~Renderer() {
    delete [] randomSeed;
}

void Renderer::Render() {
    OpenGLWindow openglwindow(screenWidth, screenHeight, true, "NERenderer");
    openglwindow.LoadOpenGLFunctions();

    Shader renderShader(VERTEX_SHADER_PATH, RENDER_FRAGMENT_SHADER_PATH);
    Shader displayShader(VERTEX_SHADER_PATH, DISPLAY_FRAGMENT_SHADER_PATH);

    Mesh mesh((float *) vertices,
              ArrayLength(vertices),
              (unsigned int *) faces,
              ArrayLength(faces));

    Texture1D sceneTexture = scene->CreateSceneTexture();

    FrameBuffer frame;

    Texture2D frameTexture(screenWidth,
                           screenHeight,
                           GL_CLAMP_TO_EDGE,
                           GL_NEAREST,
                           GL_RGBA32F,
                           GL_RGBA,
                           GL_FLOAT,
                           NULL);

    frame.BindTexture(frameTexture.GetTextureID());

    default_random_engine engine(time(0));
    for (int i = 0; i < screenWidth * screenHeight; i++) {
        randomSeed[i] = engine();
    }

    Texture2D randomSeedTexture(screenWidth,
                                screenHeight,
                                GL_CLAMP_TO_EDGE,
                                GL_NEAREST,
                                GL_R32UI,
                                GL_RED_INTEGER,
                                GL_UNSIGNED_INT,
                                (char *) randomSeed);

    int frameCount = 1;

    Image image(screenWidth, screenHeight, RGBA);

    openglwindow.RenderLoop(
        [&, scene = scene, camera = camera] (GLFWwindow *window) {
            // Process Input
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }

            /*** Render Image ***/

            // Bind Frame Buffer to Frame Buffer Texture
            glBindFramebuffer(GL_FRAMEBUFFER, frame.GetFrameBufferID());

            if (!accumulate) {
                // Clear Frame Buffer Texture
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
            }

            // Activate Frame Buffer Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, frameTexture.GetTextureID());

            // Activate Scene Texture
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_1D, sceneTexture.GetTextureID());

            // Activate Random Seed Texture
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, randomSeedTexture.GetTextureID());

            // Initialize Uniform Variables
            if (accumulate) {
                renderShader.SetUniform("accumulate", 1);
            }
            else {
                renderShader.SetUniform("accumulate", 0);
            }
            scene->SetSceneUniform(&renderShader, 1);
            camera->SetCameraUniform(&renderShader);
            renderShader.SetUniform("renderer.maxDepth", maxDepth);
            renderShader.SetUniform("renderer.russianRoulete", russianRoulete);

            renderShader.SetUniform("lastRenderResult", 0);
            renderShader.SetUniform("frameCount", frameCount);
            renderShader.SetUniform("randomSeed", 2);
            renderShader.SetUniform("extraSeed", engine());
            renderShader.SetUniform("screenGeometry",
                                    (unsigned int) screenWidth,
                                    (unsigned int) screenHeight);

            // Render Image to Frame Buffer Texture
            glUseProgram(renderShader.GetShaderProgramID());
            glBindVertexArray(mesh.GetVAO());
            glDrawElements(GL_TRIANGLES, mesh.GetFacesLength(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            /*** End Render Image ***/


            /*** Display Image ***/

            // Bind Frame Buffer to Screen
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Clear Screen
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Activate Frame Buffer Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, frameTexture.GetTextureID());

            // Initialize Uniform Variables
            displayShader.SetUniform("lastRenderResult", 0);
            displayShader.SetUniform("gamma", gamma);
            displayShader.SetUniform("screenGeometry",
                                    (unsigned int) screenWidth,
                                    (unsigned int) screenHeight);

            // Display Image on Screen
            glUseProgram(displayShader.GetShaderProgramID());
            glBindVertexArray(mesh.GetVAO());
            glDrawElements(GL_TRIANGLES, mesh.GetFacesLength(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            /*** End Display Image ***/


            // Read Render Result
            glReadPixels(0, 0, screenWidth, screenHeight, GL_RGBA, GL_UNSIGNED_BYTE, image.GetArray());

            // Increase Frame Count
            frameCount++;

            // Swap Buffers and Pull Events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    );
}
