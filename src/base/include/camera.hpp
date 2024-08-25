// camera.hpp

#pragma once

#include "../../math/include/vector3.hpp"

#include "../../opengl/include/shader.hpp"

typedef struct CameraData {
    float pos[3];

    float look[3];
    float up[3];
    float right[3];

    float screenLowerLeftCorner[3];
    float screenPlaneWidth;
    float screenPlaneHeight;

    float fov;
    float focalLength;
    float lenRadius;
} CameraData;

class Camera {
    public:
        Camera(int width,
               int height,
               Vector3 pos,
               Vector3 look,
               Vector3 up,
               float fov,
               float focalLength,
               float lenRadius); 

        CameraData GetCameraData() const;
        int GetScreenWidth() const;
        int GetScreenHeight() const;
        void SetCameraUniform(Shader *shader) const;

    private:
        CameraData cameraData;
        int screenWidth;
        int screenHeight;
};
