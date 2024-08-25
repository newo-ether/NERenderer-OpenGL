// sceneparser.cpp

#include "include/sceneparser.hpp"

#include <stdexcept>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../math/include/vector3.hpp"
#include "../math/include/spectrum.hpp"

#include "../base/include/scene.hpp"
#include "../base/include/camera.hpp"
#include "../base/include/object.hpp"
#include "../base/include/shape.hpp"
#include "../base/include/material.hpp"
#include "../renderer/include/renderer.hpp"

using namespace std;


void CameraState::Reset() {
    width = Unset;
    height = Unset;
    pos = Unset;
    look = Unset;
    up = Unset;
    fov = Unset;
    focalLength = Unset;
    lenRadius = Unset;
}

void CameraBuffer::Reset() {
    width = 0;
    height = 0;
    pos = Vector3(0,0,0);
    look = Vector3(0,0,0);
    up = Vector3(0,0,0);
    fov = 0;
    focalLength = 0;
    lenRadius = 0;
}

void RenderOptionState::Reset() {
    maxDepth = Unset;
    russianRoulete = Unset;
    gamma = Unset;
    accumulate = Unset;
}

void RenderOptionBuffer::Reset() {
    maxDepth = 0;
    russianRoulete = 0;
    gamma = 0;
    accumulate = false;
}

void ShapeState::Reset() {
    type = Unset;
    p0 = Unset;
    radius = Unset;
    p1 = Unset;
    reverseNormal = Unset;
    p2 = Unset;
}

void ShapeBuffer::Reset() {
    type = TypeSphere;
    p0 = Vector3(0,0,0);
    radius = 0;
    p1 = Vector3(0,0,0);
    reverseNormal = false;
    p2 = Vector3(0,0,0);
}

void MaterialState::Reset() {
    type = Unset;
    kd = Unset;
    glowSpectrum = Unset;
}

void MaterialBuffer::Reset() {
    type = TypeLambertian;
    kd = Spectrum(0,0,0);
    glowSpectrum = Spectrum(0,0,0);
}

SceneParser::SceneParser() {}

unique_ptr<Renderer> SceneParser::Parse(const string &filename) {
    ifstream sceneFile;

    // Open Scene File
    sceneFile.open(filename);
    if (!sceneFile.is_open()) { 
        cout << "[Error] SceneParser: Failed to open scene file." << endl; 
        exit(-1);
    } 

    // Get Scene String
    stringstream sceneStream;
    sceneStream << sceneFile.rdbuf();
    string sceneString = sceneStream.str();
    sceneFile.close();

    // Parse Scene File
    state = OutToken;
    context = None;

    isCameraSet = false;
    isRenderOptionSet = false;
    isSceneSet = false;

    rowNumber = 1;
    columnNumber = 1;

    string currentToken;
    shared_ptr<Scene> scene(new Scene());

    for (char c : sceneString) {
        if (IsEmpty(c)) {
            if (state == InToken) {
                if (context == InCamera) {
                    HandleCameraToken(currentToken);
                }
                else if (context == InRenderOption) {
                    HandleRenderOptionToken(currentToken);
                }
                else if (context == InShape) {
                    HandleShapeToken(currentToken);
                }
                else if (context == InMaterial) {
                    HandleMaterialToken(currentToken);
                }
                else if (context == InScene) {
                    if (currentToken == "ObjectBegin") {
                        isShapeSet = false;
                        isMaterialSet = false;
                        context = InObject;
                    }
                    else if (currentToken == "SceneEnd") {
                        if (!isSceneSet) {
                            FieldUnsetError("Scene");
                        }
                        context = None;
                    }
                    else {
                        UnexpectedKeywordError(context, currentToken);
                    }
                }
                else if (context == InObject) {
                    if (currentToken == "ShapeBegin") {
                        if (isShapeSet) {
                            RedefinitionError("Shape");
                        }
                        else {
                            shapeState.Reset();
                            shapeBuffer.Reset();
                            context = InShape;
                        }
                    }
                    else if (currentToken == "MaterialBegin") {
                        if (isMaterialSet) {
                            RedefinitionError("Material");
                        }
                        else {
                            materialState.Reset();
                            materialBuffer.Reset();
                            context = InMaterial;
                        }
                    }
                    else if (currentToken == "ObjectEnd") {
                        if (!isShapeSet) {
                            FieldUnsetError("Shape");
                        }
                        if (!isMaterialSet) {
                            FieldUnsetError("Material");
                        }
                        Shape shape;
                        Material material;
                        if (shapeBuffer.type == TypeSphere) {
                            shape = Sphere(shapeBuffer.p0,
                                           shapeBuffer.radius,
                                           shapeBuffer.reverseNormal);
                        }
                        else if (shapeBuffer.type == TypeTriangle) {
                            shape = Triangle(shapeBuffer.p0,
                                             shapeBuffer.p1,
                                             shapeBuffer.p2,
                                             shapeBuffer.reverseNormal);
                        }
                        if (materialBuffer.type == TypeLambertian) {
                            material = Lambertian(materialBuffer.kd,
                                                  materialBuffer.glowSpectrum);
                        }

                        scene->AddObject(Object(shape, material));
                        shapeState.Reset();
                        shapeBuffer.Reset();
                        materialState.Reset();
                        materialBuffer.Reset();
                        isSceneSet = true;
                        isShapeSet = false;
                        isMaterialSet = false;
                        context = InScene;
                    }
                    else {
                        UnexpectedKeywordError(context, currentToken);
                    }
                }
                else if (context == None) {
                    if (currentToken == "CameraBegin") {
                        if (!isCameraSet) {
                            cameraState.Reset();
                            cameraBuffer.Reset();
                            context = InCamera;
                        }
                        else {
                            RedefinitionError("Camera");
                        }
                    }
                    else if (currentToken == "RenderOptionBegin") {
                        if (!isRenderOptionSet) {
                            renderOptionState.Reset();
                            renderOptionBuffer.Reset();
                            context = InRenderOption;
                        }
                        else {
                            RedefinitionError("RenderOption");
                        }
                    }
                    else if (currentToken == "SceneBegin") {
                        if (!isSceneSet) {
                            context = InScene;
                        }
                        else {
                            RedefinitionError("Scene");
                        }
                    }
                    else {
                        UnexpectedKeywordError(context, currentToken);
                    }
                }
                currentToken.clear();
                state = OutToken;
            }
            if (c == '\n') {
                state = OutToken;
                columnNumber = 0;
                rowNumber++;
            }
        }
        else {
            if (state == OutToken) {
                if (c == '#') {
                    state = Comment;
                }
                else {
                    currentToken.clear();
                    currentToken += c;
                    state = InToken;
                }
            }
            else if (state == InToken) {
                currentToken += c;
            }
        }
        columnNumber++;
    }

    // Return Data
    if (!isCameraSet) {
        FieldUnsetError("Camera");
    }
    if (!isRenderOptionSet) {
        FieldUnsetError("RenderOption");
    }
    if (!isSceneSet) {
        FieldUnsetError("Scene");
    }

    shared_ptr<Camera>
        camera(new Camera(cameraBuffer.width,
                          cameraBuffer.height,
                          cameraBuffer.pos,
                          cameraBuffer.look,
                          cameraBuffer.up,
                          cameraBuffer.fov,
                          cameraBuffer.focalLength,
                          cameraBuffer.lenRadius));

    return unique_ptr<Renderer>
           (new Renderer(scene,
                         camera,
                         renderOptionBuffer.maxDepth,
                         renderOptionBuffer.russianRoulete,
                         renderOptionBuffer.gamma,
                         renderOptionBuffer.accumulate));
}

void SceneParser::HandleCameraToken(const string &str) {
    if (cameraState.width == Setting) {
        cameraBuffer.width = StringToFloat(str, "width");
        cameraState.width = Set;
    }
    else if (cameraState.height == Setting) {
        cameraBuffer.height = StringToFloat(str, "height");
        cameraState.height = Set;
    }
    else if (cameraState.pos == Setting) {
        cameraBuffer.pos = StringToVector3(str, "pos");
        cameraState.pos = Set;
    }
    else if (cameraState.look == Setting) {
        cameraBuffer.look = StringToVector3(str, "look");
        cameraState.look = Set;
    }
    else if (cameraState.up == Setting) {
        cameraBuffer.up = StringToVector3(str, "up");
        cameraState.up = Set;
    }
    else if (cameraState.fov == Setting) {
        cameraBuffer.fov = StringToFloat(str, "fov");
        cameraState.fov = Set;
    }
    else if (cameraState.focalLength == Setting) {
        cameraBuffer.focalLength = StringToFloat(str, "focalLength");
        cameraState.focalLength = Set;
    }
    else if (cameraState.lenRadius == Setting) {
        cameraBuffer.lenRadius = StringToFloat(str, "lenRadius");
        cameraState.lenRadius = Set;
    }
    else {
        if (str == "width") {
            if (cameraState.width == Set) {
                RedefinitionError("width");
            }
            else {
                cameraState.width = Setting;
            }
        }
        else if (str == "height") {
            if (cameraState.height == Set) {
                RedefinitionError("height");
            }
            else {
                cameraState.height = Setting;
            }
        }
        else if (str == "pos") {
            if (cameraState.pos == Set) {
                RedefinitionError("pos");
            }
            else {
                cameraState.pos = Setting;
            }
        }
        else if (str == "look") {
            if (cameraState.look == Set) {
                RedefinitionError("look");
            }
            else {
                cameraState.look = Setting;
            }
        }
        else if (str == "up") {
            if (cameraState.up == Set) {
                RedefinitionError("up");
            }
            else {
                cameraState.up = Setting;
            }
        }
        else if (str == "fov") {
            if (cameraState.fov == Set) {
                RedefinitionError("fov");
            }
            else {
                cameraState.fov = Setting;
            }
        }
        else if (str == "focalLength") {
            if (cameraState.focalLength == Set) {
                RedefinitionError("focalLength");
            }
            else {
                cameraState.focalLength = Setting;
            }
        }
        else if (str == "lenRadius") {
            if (cameraState.lenRadius == Set) {
                RedefinitionError("lenRadius");
            }
            else {
                cameraState.lenRadius = Setting;
            }
        }
        else if (str == "CameraEnd") {
            if (cameraState.width == Unset) {
                ArgumentUnsetError("CameraEnd", "width");
            }
            if (cameraState.height == Unset) {
                ArgumentUnsetError("CameraEnd", "height");
            }
            if (cameraState.pos == Unset) {
                ArgumentUnsetError("CameraEnd", "pos");
            }
            if (cameraState.look == Unset) {
                ArgumentUnsetError("CameraEnd", "look");
            }
            if (cameraState.up == Unset) {
                ArgumentUnsetError("CameraEnd", "up");
            }
            if (cameraState.fov == Unset) {
                ArgumentUnsetError("CameraEnd", "fov");
            }
            if (cameraState.focalLength == Unset) {
                ArgumentUnsetError("CameraEnd", "focalLength");
            }
            if (cameraState.lenRadius == Unset) {
                ArgumentUnsetError("CameraEnd", "lenRadius");
            }
            isCameraSet = true;
            context = None;
        }
        else {
            UnexpectedKeywordError(context, str);
        }
    }
}

void SceneParser::HandleRenderOptionToken(const string &str) {
    if (renderOptionState.maxDepth == Setting) {
        renderOptionBuffer.maxDepth = StringToInt(str, "maxDepth");
        renderOptionState.maxDepth = Set;
    }
    else if (renderOptionState.russianRoulete == Setting) {
        renderOptionBuffer.russianRoulete = StringToFloat(str, "russianRoulete");
        renderOptionState.russianRoulete = Set;
    }
    else if (renderOptionState.gamma == Setting) {
        renderOptionBuffer.gamma = StringToFloat(str, "gamma");
        renderOptionState.gamma = Set;
    }
    else if (renderOptionState.accumulate == Setting) {
        renderOptionBuffer.accumulate = StringToBool(str, "accumulate");
        renderOptionState.accumulate = Set;
    }
    else {
        if (str == "maxDepth") {
            if (renderOptionState.maxDepth == Set) {
                RedefinitionError("maxDepth");
            }
            else {
                renderOptionState.maxDepth = Setting;
            }
        }
        else if (str == "russianRoulete") {
            if (renderOptionState.russianRoulete == Set) {
                RedefinitionError("russianRoulete");
            }
            else {
                renderOptionState.russianRoulete = Setting;
            }
        }
        else if (str == "gamma") {
            if (renderOptionState.gamma == Set) {
                RedefinitionError("gamma");
            }
            else {
                renderOptionState.gamma = Setting;
            }
        }
        else if (str == "accumulate") {
            if (renderOptionState.accumulate == Set) {
                RedefinitionError("accumulate");
            }
            else {
                renderOptionState.accumulate = Setting;
            }
        }
        else if (str == "RenderOptionEnd") {
            if (renderOptionState.maxDepth == Unset) {
                ArgumentUnsetError("RenderOptionEnd", "maxDepth");
            }
            if (renderOptionState.russianRoulete == Unset) {
                ArgumentUnsetError("RenderOptionEnd", "russianRoulete");
            }
            if (renderOptionState.gamma == Unset) {
                ArgumentUnsetError("RenderOptionEnd", "gamma");
            }
            if (renderOptionState.accumulate == Unset) {
                ArgumentUnsetError("RenderOptionEnd", "accumulate");
            }
            isRenderOptionSet = true;
            context = None;
        }
        else {
            UnexpectedKeywordError(context, str);
        }
    }
}

void SceneParser::HandleShapeToken(const string &str) {
    if (shapeState.type == Unset) {
        if (str == "type") {
            shapeState.type = Setting;
        }
        else {
            ArgumentUnsetError(str, "type");
        }
    }
    else if (shapeState.type == Setting) {
        if (str == "Sphere") {
            shapeBuffer.type = TypeSphere;
            shapeState.type = Set;
        }
        else if (str == "Triangle") {
            shapeBuffer.type = TypeTriangle;
            shapeState.type = Set;
        }
        else {
            ArgumentInvalidError(str, "type");
        }
    }
    else {
        if (shapeBuffer.type == TypeSphere) {
            if (shapeState.p0 == Setting) {
                shapeBuffer.p0 = StringToVector3(str, "origin");
                shapeState.p0 = Set;
            }
            else if (shapeState.radius == Setting) {
                shapeBuffer.radius = StringToFloat(str, "radius");
                shapeState.radius = Set;
            }
            else if (shapeState.reverseNormal == Setting) {
                shapeBuffer.reverseNormal = StringToBool(str, "reverseNormal");
                shapeState.reverseNormal = Set;
            }
            else {
                if (str == "origin") {
                    if (shapeState.p0 == Set) {
                        RedefinitionError("origin");
                    }
                    else {
                        shapeState.p0 = Setting;
                    }
                }
                else if (str == "radius") {
                    if (shapeState.radius == Set) {
                        RedefinitionError("radius");
                    }
                    else {
                        shapeState.radius = Setting;
                    }
                }
                else if (str == "reverseNormal") {
                    if (shapeState.reverseNormal == Set) {
                        RedefinitionError("reverseNormal");
                    }
                    else {
                        shapeState.reverseNormal = Setting;
                    }
                }
                else if (str == "ShapeEnd") {
                    if (shapeState.p0 == Unset) {
                        ArgumentUnsetError("ShapeEnd", "origin");
                    }
                    if (shapeState.radius == Unset) {
                        ArgumentUnsetError("ShapeEnd", "radius");
                    }
                    if (shapeState.reverseNormal == Unset) {
                        ArgumentUnsetError("ShapeEnd", "reverseNormal");
                    }
                    isShapeSet = true;
                    context = InObject;
                }
                else {
                    UnexpectedKeywordError(context, str);
                }
            }
        }
        else if (shapeBuffer.type == TypeTriangle) {
            if (shapeState.p0 == Setting) {
                shapeBuffer.p0 = StringToVector3(str, "p0");
                shapeState.p0 = Set;
            }
            else if (shapeState.p1 == Setting) {
                shapeBuffer.p1 = StringToVector3(str, "p1");
                shapeState.p1 = Set;
            }
            else if (shapeState.p2 == Setting) {
                shapeBuffer.p2 = StringToVector3(str, "p2");
                shapeState.p2 = Set;
            }
            else if (shapeState.reverseNormal == Setting) {
                shapeBuffer.reverseNormal = StringToBool(str, "reverseNormal");
                shapeState.reverseNormal = Set;
            }
            else {
                if (str == "p0") {
                    if (shapeState.p0 == Set) {
                        RedefinitionError("p0");
                    }
                    else {
                        shapeState.p0 = Setting;
                    }
                }
                else if (str == "p1") {
                    if (shapeState.p1 == Set) {
                        RedefinitionError("p1");
                    }
                    else {
                        shapeState.p1 = Setting;
                    }
                }
                else if (str == "p2") {
                    if (shapeState.p2 == Set) {
                        RedefinitionError("p2");
                    }
                    else {
                        shapeState.p2 = Setting;
                    }
                }
                else if (str == "reverseNormal") {
                    if (shapeState.reverseNormal == Set) {
                        RedefinitionError("reverseNormal");
                    }
                    else {
                        shapeState.reverseNormal = Setting;
                    }
                }
                else if (str == "ShapeEnd") {
                    if (shapeState.p0 == Unset) {
                        ArgumentUnsetError("ShapeEnd", "p0");
                    }
                    if (shapeState.p1 == Unset) {
                        ArgumentUnsetError("ShapeEnd", "p1");
                    }
                    if (shapeState.p2 == Unset) {
                        ArgumentUnsetError("ShapeEnd", "p2");
                    }
                    if (shapeState.reverseNormal == Unset) {
                        ArgumentUnsetError("ShapeEnd", "reverseNormal");
                    }
                    isShapeSet = true;
                    context = InObject;
                }
                else {
                    UnexpectedKeywordError(context, str);
                }
            }
        }
    }
}

void SceneParser::HandleMaterialToken(const string &str) {
    if (materialState.type == Unset) {
        if (str == "type") {
            materialState.type = Setting;
        }
        else {
            ArgumentUnsetError(str, "type");
        }
    }
    else if (materialState.type == Setting) {
        if (str == "Lambertian") {
            materialBuffer.type = TypeLambertian;
            materialState.type = Set;
        }
        else {
            ArgumentInvalidError(str, "type");
        }
    }
    else {
        if (materialBuffer.type == TypeLambertian) {
            if (materialState.kd == Setting) {
                materialBuffer.kd = StringToSpectrum(str, "kd");
                materialState.kd = Set;
            }
            else if (materialState.glowSpectrum == Setting) {
                materialBuffer.glowSpectrum = StringToSpectrum(str, "glowSpectrum");
                materialState.glowSpectrum = Set;
            }
            else {
                if (str == "kd") {
                    if (materialState.kd == Set) {
                        RedefinitionError("kd");
                    }
                    else {
                        materialState.kd = Setting;
                    }
                }
                else if (str == "glowSpectrum") {
                    if (materialState.glowSpectrum == Set) {
                        RedefinitionError("glowSpectrum");
                    }
                    else {
                        materialState.glowSpectrum = Setting;
                    }
                }
                else if (str == "MaterialEnd") {
                    if (materialState.kd == Unset) {
                        ArgumentUnsetError("MaterialEnd", "kd");
                    }
                    if (materialState.glowSpectrum == Unset) {
                        ArgumentUnsetError("MaterialEnd", "glowSpectrum");
                    }
                    isMaterialSet = true;
                    context = InObject;
                }
                else {
                    UnexpectedKeywordError(context, str);
                }
            }
        }
    }
}

bool SceneParser::IsEmpty(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\0';
}

int SceneParser::StringToInt(const string &s, const string &argumentName) {
    int num = 0;
    bool start = true;
    if (s.size() < 1) {
        ArgumentInvalidError(s, argumentName);
    }
    for (char c : s) {
        if (start) {
            if (c != '+' && c != '-' && (c < '0' || c > '9')) {
                ArgumentInvalidError(s, argumentName);
            }
            start = false;
        }
        else {
            if (c < '0' || c > '9') {
                ArgumentInvalidError(s, argumentName);
            }
        }
    }
    try {
        num = stoi(s);
    }
    catch (const invalid_argument &) {
        ArgumentInvalidError(s, argumentName);
    }
    catch (const out_of_range &) {
        ArgumentOutOfRangeError(s, argumentName);
    }
    return num;
}

bool SceneParser::StringToBool(const string &s, const string &argumentName) {
    bool value = 0;
    if (s == "true") {
        value = true;
    }
    else if (s == "false") {
        value = false;
    }
    else {
        ArgumentInvalidError(s, argumentName);
    }
    return value;
}

float SceneParser::StringToFloat(const string &s, const string &argumentName) {
    float num = 0.0;
    bool start = true;
    bool metDot = false;
    if (s.size() < 1) {
        ArgumentInvalidError(s, argumentName);
    }
    for (char c : s) {
        if (start) {
            if (c == '.') {
                metDot = true;
            }
            else if (c != '+' && c != '-' && (c < '0' || c > '9')) {
                ArgumentInvalidError(s, argumentName);
            }
            start = false;
        }
        else {
            if (c == '.') {
                if (metDot) {
                    ArgumentInvalidError(s, argumentName);
                }
                else {
                    metDot = true;
                }
            }
            else if (c < '0' || c > '9') {
                ArgumentInvalidError(s, argumentName);
            }
        }
    }
    try {
        num = stof(s);
    }
    catch (const invalid_argument &) {
        ArgumentInvalidError(s, argumentName);
    }
    catch (const out_of_range &) {
        ArgumentOutOfRangeError(s, argumentName);
    }
    return num;
}

Vector3 SceneParser::StringToVector3(const string &s, const string &argumentName) {
    if (s.size() < 7 || *(s.begin()) != '[' || *(--s.end()) != ']') {
        ArgumentInvalidError(s, argumentName);
    }
    string item[3];
    string temp = s.substr(1, s.size() - 2);
    int count = 0;
    for (char c : temp) {
        if (count > 2) {
            ArgumentInvalidError(s, argumentName);
        }
        if (c == ',') {
            count++;
        }
        else {
            item[count] += c;
        }
    }
    if (count != 2) {
        ArgumentInvalidError(s, argumentName);
    }
    return Vector3(StringToFloat(item[0], argumentName),
                   StringToFloat(item[1], argumentName),
                   StringToFloat(item[2], argumentName));
}

Spectrum SceneParser::StringToSpectrum(const string &s, const string &argumentName) {
    if (s.size() < 7 || *(s.begin()) != '[' || *(--s.end()) != ']') {
        ArgumentInvalidError(s, argumentName);
    }
    string item[3];
    string temp = s.substr(1, s.size() - 2);
    int count = 0;
    for (char c : temp) {
        if (count > 2) {
            ArgumentInvalidError(s, argumentName);
        }
        if (c == ',') {
            count++;
        }
        else {
            item[count] += c;
        }
    }
    if (count != 2) {
        ArgumentInvalidError(s, argumentName);
    }
    return Spectrum(StringToFloat(item[0], argumentName),
                    StringToFloat(item[1], argumentName),
                    StringToFloat(item[2], argumentName));
}

string SceneParser::GetContextString(Context context) {
    switch (context) {
        case None: return "";
        case InCamera: return "Camera";
        case InRenderOption: return "RenderOption";
        case InScene: return "Scene";
        case InObject: return "Object";
        case InShape: return "Shape";
        case InMaterial: return "Material";
    }
}

void SceneParser::UnexpectedKeywordError(Context context, const string &token) {
    string field = GetContextString(context);
    if (field == "") {
        cout << "[Error] SceneParser: Syntax Error at ("
             << rowNumber << ", " << columnNumber << "): Unexpected keyword \""
             << token << "\"." << endl;
    }
    else {
        cout << "[Error] SceneParser: Syntax Error at ("
             << rowNumber << ", " << columnNumber << "): Unexpected keyword \""
             << token << "\" encountered in \"" 
             << field << "\" field." << endl;
    }
    exit(-1);
}

void SceneParser::RedefinitionError(const string &name) {
    cout << "[Error] SceneParser: Redefinition Error at (" 
         << rowNumber << ", " << columnNumber
         << "): Redefinition of \"" << name << "\"." << endl;
    exit(-1);
}

void SceneParser::FieldUnsetError(const string &fieldName) {
    cout << "[Error] SceneParser: Field Unset Error at ("
         << rowNumber << ", " << columnNumber << "): Field \""
         << fieldName << "\" unset." << endl;
    exit(-1);
}

void SceneParser::ArgumentUnsetError(const string &token, const string &argumentName) {
    cout << "[Error] SceneParser: Argument Unset Error at ("
         << rowNumber << ", " << columnNumber << "): Argument \""
         << argumentName << "\" unset when \""
         << token << "\" encountered." << endl;
    exit(-1);
}

void SceneParser::ArgumentInvalidError(const string &token, const string &argumentName) {
    cout << "[Error] SceneParser: Argument Invalid Error at ("
         << rowNumber << ", " << columnNumber << "): Invalid argument \""
         << token << "\" encountered when setting \""
         << argumentName << "\"." << endl;
    exit(-1);
}

void SceneParser::ArgumentOutOfRangeError(const string &token, const string &argumentName) {
    cout << "[Error] SceneParser: Argument Out of Range Error at ("
         << rowNumber << ", " << columnNumber << "): Out-of-range argument \""
         << token << "\" encountered when setting \""
         << argumentName << "\"." << endl;
    exit(-1);
}
