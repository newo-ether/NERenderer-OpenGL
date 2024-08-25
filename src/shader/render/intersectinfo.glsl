// intersectinfo.glsl

#include "material.glsl"

struct IntersectInfo {
    bool isHit;
    float tHit;
    vec3 hitPoint;
    vec3 hitNormal;
    vec3 incomeDir;
    Material hitMaterial;
};

IntersectInfo IntersectInfoInit(float tHit,
                                vec3 hitPoint,
                                vec3 hitNormal,
                                vec3 incomeDir,
                                Material material) {

    IntersectInfo isect;
    isect.isHit = true;
    isect.tHit = tHit;
    isect.hitPoint = hitPoint;
    isect.hitNormal = hitNormal;
    isect.incomeDir = incomeDir;
    isect.hitMaterial = material;

    return isect;
}

IntersectInfo IntersectInfoInitNone() {
    IntersectInfo isect;
    isect.isHit = false;
    isect.tHit = INF;
    isect.hitPoint = vec3(0.0f, 0.0f, 0.0f);
    isect.hitNormal = vec3(0.0f, 0.0f, 0.0f);
    isect.incomeDir = vec3(0.0f, 0.0f, 0.0f);
    isect.hitMaterial = MaterialInitEmpty();

    return isect;
}
