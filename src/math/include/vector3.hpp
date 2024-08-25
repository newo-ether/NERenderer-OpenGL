// vector3.hpp

#pragma once

class Vector3 {
    public:
        Vector3();
        Vector3(float x, float y, float z);
        float x, y, z;

        Vector3 operator+(Vector3 v);
        Vector3 operator-(Vector3 v);
        Vector3 operator-();
        float   operator*(Vector3 v);
        Vector3 operator*(float n);
        Vector3 operator/(float n);
        float   operator[](unsigned int i);

        Vector3 & operator+=(Vector3 v);
        Vector3 & operator-=(Vector3 v);
        Vector3 & operator*=(Vector3 v);
        Vector3 & operator*=(float n);
        Vector3 & operator/=(float n);

        static Vector3 Abs(Vector3 v);
        static float   Dot(Vector3 v1, Vector3 v2);
        static float   AbsDot(Vector3 v1, Vector3 v2);
        static Vector3 Cross(Vector3 v1, Vector3 v2);
        static Vector3 Normalize(Vector3 v);

        static float LengthSquared(Vector3 v);
        static float Length(Vector3 v);
};
