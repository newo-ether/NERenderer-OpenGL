// renderer.hpp

#pragma once

#include "../../base/include/camera.hpp"
#include "../../base/include/scene.hpp"

#include <memory>

class Renderer {
    public:
        Renderer(shared_ptr<Scene> scene,
                 shared_ptr<Camera> camera,
                 int maxDepth,
                 float russianRoulete,
                 float gamma,
                 bool accumulate);

        ~Renderer();

        void Render();

    private:
        static const float vertices[];
        static const unsigned int faces[];

        int screenWidth;
        int screenHeight;

        shared_ptr<Scene> scene;
        shared_ptr<Camera> camera;

        int maxDepth;
        float russianRoulete;
        float gamma;
        bool accumulate;

        unsigned int *randomSeed;
};
