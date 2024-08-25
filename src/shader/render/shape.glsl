// shape.glsl

#include "intersectinfo.glsl"
#include "material.glsl"
#include "objectptr.glsl"
#include "ray.glsl"

struct Shape {
    // Pack 1
    float type;
    vec3 p0; // For Sphere Position

    // Pack 2
    float radius; // For Sphere
    vec3 p1;

    // Pack 3
    float reverseNormal;
    vec3 p2;
};

Shape ShapeInit(ObjectPtr objPtr) {
    vec4 shapeData[3];
    float index = float(objPtr.index);

    shapeData[0] = texture1D(objectList, (float(index) + 0.0f + 0.5f) / float(packNum));
    shapeData[1] = texture1D(objectList, (float(index) + 1.0f + 0.5f) / float(packNum));
    shapeData[2] = texture1D(objectList, (float(index) + 2.0f + 0.5f) / float(packNum));

    Shape shape;
    shape.type = shapeData[0].x;
    shape.p0 = shapeData[0].yzw;
    shape.radius = shapeData[1].x;
    shape.p1 = shapeData[1].yzw;
    shape.reverseNormal = shapeData[2].x;
    shape.p2 = shapeData[2].yzw;

    return shape;
}

Shape ShapeInitEmpty() {
    Shape shape;
    shape.type = 0.0f;
    shape.p0 = vec3(0.0f, 0.0f, 0.0f);
    shape.radius = 0.0f;
    shape.p1 = vec3(0.0f, 0.0f, 0.0f);
    shape.reverseNormal = 0.0f;
    shape.p2 = vec3(0.0f, 0.0f, 0.0f);

    return shape;
}

IntersectInfo SphereIntersect(vec3 origin, float radius, Ray ray, bool reverseNormal) {
    vec3 ro = ray.origin;
    vec3 so = origin;
    vec3 ro2so = ro - so;
    vec3 d = ray.dir;

    // Use this if the ray direction is not normalized.
    //
    // float a = dot(d, d);
    // float b = 2 * dot(d, ro2so);
    // float c = dot(ro2so, ro2so) - radius * radius;

    float a = 1.0f;
    float b = 2 * dot(d, ro2so);
    float c = dot(ro2so, ro2so) - radius * radius;

    float delta = b * b - 4 * a * c;
    if (delta > 0) {
        float inv2a = 1.0f / (2.0f * a);
        float sqrtDelta = sqrt(delta);
        float t1 = (-b - sqrtDelta) * inv2a;
        float t2 = (-b + sqrtDelta) * inv2a;
        t1 = min(t1, t2);
        t2 = max(t1, t2);
        if (t1 > ray.tMin && t1 < ray.tMax) {
            vec3 p = RayAt(ray, t1);
            vec3 normal = (p - so) / radius;
            normal = reverseNormal ? -normal : normal;
            p += normal * EPSILON;
            // Use this if the ray direction is not normalized.
            // vec3 income = normalize(d);
            vec3 income = d;
            return IntersectInfoInit(t1, p, normal, income, MaterialInitEmpty());
        }
        else if (t2 > ray.tMin && t2 < ray.tMax) {
            vec3 p = RayAt(ray, t2);
            vec3 normal = (p - so) / radius;
            normal = reverseNormal ? -normal : normal;
            p += normal * EPSILON;
            // Use this if the ray direction is not normalized.
            // vec3 income = normalize(d);
            vec3 income = d;
            return IntersectInfoInit(t2, p, normal, income, MaterialInitEmpty());
        }
        else {
            return IntersectInfoInitNone();
        }
    }
    else {
        return IntersectInfoInitNone();
    }
}

bool SphereIsIntersect(vec3 origin, float radius, Ray ray) {
    vec3 ro = ray.origin;
    vec3 so = origin;
    vec3 ro2so = ro - so;
    vec3 d = ray.dir;

    // Use this if the ray direction is not normalized.
    //
    // float a = dot(d, d);
    // float b = 2 * dot(d, ro2so);
    // float c = dot(ro2so, ro2so) - radius * radius;

    float a = 1.0f;
    float b = 2 * dot(d, ro2so);
    float c = dot(ro2so, ro2so) - radius * radius;

    float delta = b * b - 4 * a * c;
    if (delta > 0) {
        float inv2a = 1.0f / (2.0f * a);
        float sqrtDelta = sqrt(delta);
        float t1 = (-b - sqrtDelta) * inv2a;
        float t2 = (-b + sqrtDelta) * inv2a;
        t1 = min(t1, t2);
        t2 = max(t1, t2);
        if ((t1 > ray.tMin && t1 < ray.tMax) || (t2 > ray.tMin && t2 < ray.tMax)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

IntersectInfo TriangleIntersect(vec3 p0, vec3 p1, vec3 p2, Ray r, bool reverseNormal) {
    return IntersectInfoInitNone();
}

bool TriangleIsIntersect(vec3 p0, vec3 p1, vec3 p2, Ray r) {
    return false;
}

IntersectInfo ShapeIntersect(Shape shape, Ray ray) {
    bool reverseNormal = shape.reverseNormal == 0.0f ? false : true;
    if (shape.type == 0.0f) {
        return SphereIntersect(shape.p0, shape.radius, ray, reverseNormal);
    }
    else {
        return TriangleIntersect(shape.p0, shape.p1, shape.p2, ray, reverseNormal);
    }
}

bool ShapeIsIntersect(Shape shape, Ray ray) {
    bool reverseNormal = shape.reverseNormal == 0.0f ? false : true;
    if (shape.type == 0.0f) {
        return SphereIsIntersect(shape.p0, shape.radius, ray);
    }
    else {
        return TriangleIsIntersect(shape.p0, shape.p1, shape.p2, ray);
    }
}
