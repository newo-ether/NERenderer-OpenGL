// vector3.cpp

#include "include/vector3.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Vector3::Vector3(): x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z) {}

Vector3 Vector3::operator+(Vector3 v) {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(Vector3 v) {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator-() {
    return Vector3(-x, -y, -z);
}

float Vector3::operator*(Vector3 v) {
    return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::operator*(float n)  {
    return Vector3(x * n, y * n, z * n);
}

Vector3 Vector3::operator/(float n) {
    return Vector3(x / n, y / n, z / n);
}

float Vector3::operator[](unsigned int i) {
    switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default:
            cout << "[Error] Vector3: Index out of range." << endl;
            exit(-1);
    }
}

Vector3 & Vector3::operator+=(Vector3 v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3 & Vector3::operator-=(Vector3 v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3 & Vector3::operator*=(Vector3 v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

Vector3 & Vector3::operator*=(float n) {
    x *= n;
    y *= n;
    z *= n;
    return *this;
}

Vector3 & Vector3::operator/=(float n) {
    x /= n;
    y /= n;
    z /= n;
    return *this;
}

Vector3 Vector3::Abs(Vector3 v) {
    return Vector3(abs(v.x), abs(v.y), abs(v.z));
}

float Vector3::Dot(Vector3 v1, Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z + v2.z;
}

float Vector3::AbsDot(Vector3 v1, Vector3 v2) {
    return abs(Dot(v1, v2));
}

Vector3 Vector3::Cross(Vector3 v1, Vector3 v2) {
    return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

Vector3 Vector3::Normalize(Vector3 v) {
    return v / Length(v);
}

float Vector3::LengthSquared(Vector3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vector3::Length(Vector3 v) {
    return sqrt(LengthSquared(v));
}
