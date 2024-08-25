// scene.cpp

#include "include/scene.hpp"

#include "include/object.hpp"
#include "../opengl/include/shader.hpp"
#include "../opengl/include/texture1D.hpp"

#include <vector>

using namespace std;

Scene::Scene() {
    packPerObject = sizeof(ObjectData) / (4 * sizeof(float));
    scene = new vector<ObjectData>();
}

Scene::~Scene() {
    delete scene;
}

Scene & Scene::AddObject(const Object &object) {
    scene->push_back(object.GetObjectData());
    return *this;
}

Texture1D Scene::CreateSceneTexture() {
    Texture1D sceneTexture(scene->size() * packPerObject,
                           GL_CLAMP_TO_EDGE,
                           GL_NEAREST,
                           GL_RGBA32F,
                           GL_RGBA,
                           GL_FLOAT,
                           (char *) scene->data());
    return sceneTexture;
}

void Scene::SetSceneUniform(Shader *shader, int sceneTextureUnit) {
    shader->SetUniform("packPerObject", packPerObject);
    shader->SetUniform("packNum", (unsigned int) (scene->size() * packPerObject));
    shader->SetUniform("objectList", sceneTextureUnit);
}
