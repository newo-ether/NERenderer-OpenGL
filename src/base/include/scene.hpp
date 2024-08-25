// scene.hpp

#pragma once

#include "object.hpp"
#include "../../opengl/include/shader.hpp"
#include "../../opengl/include/texture1D.hpp"

#include <vector>

using namespace std;

class Scene {
    public:
        Scene();
        ~Scene();

        Scene & AddObject(const Object &object);
        Texture1D CreateSceneTexture();
        void SetSceneUniform(Shader *shader, int sceneTextureUnit);

    private:
        vector<ObjectData> *scene;
        unsigned int packPerObject;
};
