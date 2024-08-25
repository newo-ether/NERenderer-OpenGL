// objectptr.glsl

struct ObjectPtr {
    uint index;
};

ObjectPtr ObjectPtrInit(uint index) {
    ObjectPtr objPtr;
    objPtr.index = index;
    return objPtr;
}

bool ObjectPtrEOF(ObjectPtr objPtr) {
    return objPtr.index >= packNum;
}

ObjectPtr ObjectPtrNext(ObjectPtr objPtr) {
    objPtr.index += packPerObject;
    return objPtr;
}
