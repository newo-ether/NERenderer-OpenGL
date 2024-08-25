// ray.glsl

struct Ray {
    vec3 origin;
    vec3 dir;
    float tMin;
    float tMax;
};

Ray RayInit(vec3 origin, vec3 dir, float tMin, float tMax) {
    Ray ray;
    ray.origin = origin;
    ray.dir = dir;
    ray.tMin = tMin;
    ray.tMax = tMax;
    return ray;
}

vec3 RayAt(Ray ray, float t) {
    return ray.origin + ray.dir * t;
}
