// shape.hpp

#pragma once

#include "../../math/include/vector3.hpp"

#pragma pack(1)

typedef struct ShapeData {
    // Pack 1
    float type;
    float p0[3]; // For Sphere Position

    // Pack 2
    float radius; // For Sphere
    float p1[3];

    // Pack 3
    float reverseNormal;
    float p2[3];
} ShapeData;

#pragma pack()


class Shape {
    public:
        Shape();
        ShapeData GetShapeData() const;

    protected:
        ShapeData shapeData;
};

class Sphere : public Shape {
    public:
        Sphere(Vector3 pos, float radius, bool reverseNormal);
};

class Triangle : public Shape {
    public:
        Triangle(Vector3 p0, Vector3 p1, Vector3 p2, bool reverseNormal);
};
