// object.cpp

#include "include/object.hpp"

#include "include/shape.hpp"
#include "include/material.hpp"

Object::Object(const Shape &shape, const Material &material) {
    objectData.shape = shape.GetShapeData();
    objectData.material = material.GetMaterialData();
}

ObjectData Object::GetObjectData() const {
    return objectData;
}
