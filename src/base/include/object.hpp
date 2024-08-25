// object.hpp

#pragma once

#include "shape.hpp"
#include "material.hpp"

#pragma pack(1)

typedef struct ObjectData {
    struct ShapeData shape;
    struct MaterialData material;
} ObjectData;

#pragma pack()


class Object {
    public:
        Object(const Shape &shape, const Material &material);
        ObjectData GetObjectData() const;

    private:
        ObjectData objectData;
};
