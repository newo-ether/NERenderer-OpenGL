// sceneparser.hpp

#pragma once

#include <string>
#include <memory>

#include "../../math/include/vector3.hpp"
#include "../../math/include/spectrum.hpp"
#include "../../renderer/include/renderer.hpp"

using namespace std;

enum State {
    OutToken,
    InToken,
    Comment
};

enum Context {
    None,
    InCamera,
    InRenderOption,
    InScene,
    InObject,
    InShape,
    InMaterial
};

enum Flag {
    Unset,
    Setting,
    Set
};

enum ShapeType {
    TypeSphere,
    TypeTriangle
};

enum MaterialType {
    TypeLambertian
};

struct CameraState {
    void Reset();
    Flag width;
    Flag height;
    Flag pos;
    Flag look;
    Flag up;
    Flag fov;
    Flag focalLength;
    Flag lenRadius;
};

struct CameraBuffer {
    void Reset();
    int width;
    int height;
    Vector3 pos;
    Vector3 look;
    Vector3 up;
    float fov;
    float focalLength;
    float lenRadius;
};

struct RenderOptionState {
    void Reset();
    Flag maxDepth;
    Flag russianRoulete;
    Flag gamma;
    Flag accumulate;
};

struct RenderOptionBuffer {
    void Reset();
    int maxDepth;
    float russianRoulete;
    float gamma;
    bool accumulate;
};

struct ShapeState {
    void Reset();
    Flag type;
    Flag p0;
    Flag radius;
    Flag p1;
    Flag reverseNormal;
    Flag p2;
};

struct ShapeBuffer {
    void Reset();
    ShapeType type;
    Vector3 p0;
    float radius;
    Vector3 p1;
    bool reverseNormal;
    Vector3 p2;
};

struct MaterialState {
    void Reset();
    Flag type;
    Flag kd;
    Flag glowSpectrum;
};

struct MaterialBuffer {
    void Reset();
    MaterialType type;
    Spectrum kd;
    Spectrum glowSpectrum;
};

class SceneParser {
    public:
        SceneParser();
        
        unique_ptr<Renderer> Parse(const string &filename);

    private:
        State state;
        Context context;
        CameraState cameraState;
        CameraBuffer cameraBuffer;
        RenderOptionState renderOptionState;
        RenderOptionBuffer renderOptionBuffer;
        ShapeState shapeState;
        ShapeBuffer shapeBuffer;
        MaterialState materialState;
        MaterialBuffer materialBuffer;

        bool isCameraSet, isRenderOptionSet, isSceneSet;
        bool isShapeSet, isMaterialSet;
        int rowNumber, columnNumber;

        bool IsEmpty(char c);

        int StringToInt(const string &s, const string &argumentName);
        bool StringToBool(const string &s, const string &argumentName);
        float StringToFloat(const string &s, const string &argumentName);
        Vector3 StringToVector3(const string &s, const string &argumentName);
        Spectrum StringToSpectrum(const string &s, const string &argumentName);

        string GetContextString(Context context);
        
        void HandleCameraToken(const string &str);
        void HandleRenderOptionToken(const string &str);
        void HandleShapeToken(const string &str);
        void HandleMaterialToken(const string &str);

        void UnexpectedKeywordError(Context context, const string &token);
        void RedefinitionError(const string &name);
        void FieldUnsetError(const string &fieldName);
        void ArgumentUnsetError(const string &token, const string &argumentName);
        void ArgumentInvalidError(const string &token, const string &argumentName);
        void ArgumentOutOfRangeError(const string &token, const string &argumentName);
};
