// renderer.glsl

#include "ray.glsl"
#include "scene.glsl"
#include "intersectinfo.glsl"
#include "material.glsl"
#include "random.glsl"

struct Renderer {
    int maxDepth;
    float russianRoulete;
};

uniform Renderer renderer;

vec3 PathTracing(Ray ray) {
    vec3 Lo = vec3(0.0f, 0.0f, 0.0f);
    vec3 decay = vec3(1.0f, 1.0f, 1.0f);
    float maxDepth = renderer.maxDepth;
    float rr = renderer.russianRoulete;
    float invrr = 1.0f / rr;

    for (int depth = 0; depth < maxDepth; depth++) {
        IntersectInfo isect = SceneIntersect(ray);
        if (isect.isHit) {
            Material material = isect.hitMaterial;
            if (MaterialIsGlow(material)) {
                vec3 Li = MaterialGetGlowSpectrum(material);
                Lo = Li * decay;
                break;
            }
            if (Random01() > rr) {
                Lo = vec3(0.0f, 0.0f, 0.0f);
                break;
            }
            vec3 newDir = RandomHemisphereDir(isect.hitNormal);
            ray = RayInit(isect.hitPoint, newDir, 0.0f, INF);
            float invPdf = TWOPI;
            decay *= MaterialBSDF(material, newDir, isect.incomeDir, isect.hitNormal)
                     * dot(newDir, isect.hitNormal) * invPdf * invrr;
        }
        else {
            Lo = vec3(0.0f, 0.0f, 0.0f);
            break;
        }
    }
    return Lo;
}
