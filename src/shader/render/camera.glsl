// camera.glsl

#include "ray.glsl"
#include "random.glsl"

struct Camera {
    vec3 pos;

    vec3 look;
    vec3 up;
    vec3 right;

    vec3 screenLowerLeftCorner;
    float screenPlaneWidth;
    float screenPlaneHeight;

    float fov;
    float focalLength;
    float lenRadius;
};

uniform Camera camera;

Ray CameraGetSampleRay(vec2 screenIndex) {
    vec2 sampleOffset = (screenIndex + 0.5 * vec2(Random11(),Random11())) / vec2(screenGeometry);
    vec3 samplePoint =  camera.screenLowerLeftCorner
                        + camera.right * sampleOffset.x * camera.screenPlaneWidth
                        + camera.up    * sampleOffset.y * camera.screenPlaneHeight;

    return RayInit(camera.pos, normalize(samplePoint - camera.pos), 0.0f, INF);
}
