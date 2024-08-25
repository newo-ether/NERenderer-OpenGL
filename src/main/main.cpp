// main.cpp

#include "../parser/include/sceneparser.hpp"
#include "../renderer/include/renderer.hpp"

#include <memory>

using namespace std;

int main() {
    SceneParser parser;
    unique_ptr<Renderer> renderer = parser.Parse("example-scene/scene.ne");
    renderer->Render();
}
