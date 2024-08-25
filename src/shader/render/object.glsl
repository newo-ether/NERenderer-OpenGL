// object.glsl

#include "shape.glsl"
#include "material.glsl"
#include "objectptr.glsl"

struct Object {
    Shape shape;
    Material material;
};

Object ObjectInit(ObjectPtr objPtr) {
    vec4 shapeData[3];
    vec4 materialData[2];
    float index = float(objPtr.index);

    shapeData[0] = texture1D(objectList, (float(index) + 0.0f + 0.5f) / float(packNum));
    shapeData[1] = texture1D(objectList, (float(index) + 1.0f + 0.5f) / float(packNum));
    shapeData[2] = texture1D(objectList, (float(index) + 2.0f + 0.5f) / float(packNum));
    materialData[0] = texture1D(objectList, (float(index) + 3.0f + 0.5f) / float(packNum));
    materialData[1] = texture1D(objectList, (float(index) + 4.0f + 0.5f) / float(packNum));

    Shape shape;
    shape.type = shapeData[0].x;
    shape.p0 = shapeData[0].yzw;
    shape.radius = shapeData[1].x;
    shape.p1 = shapeData[1].yzw;
    shape.reverseNormal = shapeData[2].x;
    shape.p2 = shapeData[2].yzw;

    Material material;
    material.type = materialData[0].x;
    material.kd = materialData[0].yzw;
    material.isGlow = materialData[1].x;
    material.glowSpectrum = materialData[1].yzw;

    Object obj;
    obj.shape = shape;
    obj.material = material;

    return obj;
}

Object ObjectInitEmpty() {
    Shape shape;
    shape.type = 0.0f;
    shape.p0 = vec3(0.0f, 0.0f, 0.0f);
    shape.radius = 0.0f;
    shape.p1 = vec3(0.0f, 0.0f, 0.0f);
    shape.reverseNormal = 0.0f;
    shape.p2 = vec3(0.0f, 0.0f, 0.0f);

    Material material;
    material.type = 0.0f;
    material.kd = vec3(0.0f, 0.0f, 0.0f);
    material.isGlow = 0.0f;
    material.glowSpectrum = vec3(0.0f, 0.0f, 0.0f);

    Object obj;
    obj.shape = shape;
    obj.material = material;

    return obj;
}
