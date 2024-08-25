// camera.cpp

#include "include/camera.hpp"

#include "../math/include/vector3.hpp"
#include "../math/include/utils.hpp"
#include "../opengl/include/shader.hpp"

#include <cmath>

Camera::Camera(int width,
               int height,
               Vector3 pos,
               Vector3 look,
               Vector3 up,
               float fov,
               float focalLength,
               float lenRadius):

               screenWidth(width),
               screenHeight(height) {

    look = Vector3::Normalize(look);
    up = Vector3::Normalize(up);

    Vector3 right = Vector3::Cross(look, up);
    Vector3 newUp = Vector3::Cross(right, look);

    float aspect = (float) width / height;
    Vector3 origin = pos + look * focalLength;
    float screenPlaneWidth = 2 * tan(TORADIAN(fov / 2)) * focalLength;
    float screenPlaneHeight = screenPlaneWidth / aspect;
    Vector3 screenLowerLeftCorner = origin + look * focalLength
                                    - right * screenPlaneWidth / 2
                                    - newUp * screenPlaneHeight / 2;

    cameraData.pos[0] = pos[0];
    cameraData.pos[1] = pos[1];
    cameraData.pos[2] = pos[2];

    cameraData.look[0] = look[0];
    cameraData.look[1] = look[1];
    cameraData.look[2] = look[2];

    cameraData.up[0] = newUp[0];
    cameraData.up[1] = newUp[1];
    cameraData.up[2] = newUp[2];

    cameraData.right[0] = right[0];
    cameraData.right[1] = right[1];
    cameraData.right[2] = right[2];

    cameraData.screenLowerLeftCorner[0] = screenLowerLeftCorner[0];
    cameraData.screenLowerLeftCorner[1] = screenLowerLeftCorner[1];
    cameraData.screenLowerLeftCorner[2] = screenLowerLeftCorner[2];

    cameraData.screenPlaneWidth = screenPlaneWidth;
    cameraData.screenPlaneHeight = screenPlaneHeight;

    cameraData.fov = fov;
    cameraData.focalLength = focalLength;
    cameraData.lenRadius = lenRadius;
}

CameraData Camera::GetCameraData() const {
    return cameraData;
}

int Camera::GetScreenWidth() const {
    return screenWidth;
}

int Camera::GetScreenHeight() const {
    return screenHeight;
}

void Camera::SetCameraUniform(Shader *shader) const {
    shader->SetUniform("camera.pos", (float *) &cameraData.pos, 3);

    shader->SetUniform("camera.look", (float *) &cameraData.look, 3);
    shader->SetUniform("camera.up", (float *) &cameraData.up, 3);
    shader->SetUniform("camera.right", (float *) &cameraData.right, 3);

    shader->SetUniform("camera.screenLowerLeftCorner", (float *) &cameraData.screenLowerLeftCorner, 3);

    shader->SetUniform("camera.screenPlaneWidth", cameraData.screenPlaneWidth);
    shader->SetUniform("camera.screenPlaneHeight", cameraData.screenPlaneHeight);

    shader->SetUniform("camera.fov", cameraData.fov);
    shader->SetUniform("camera.focalLength", cameraData.focalLength);
    shader->SetUniform("camera.lenRadius", cameraData.lenRadius);
}
