// spectrum.cpp

#include "include/spectrum.hpp"

#include <cmath>
#include <iostream>

using namespace std;

Spectrum::Spectrum(): r(0), g(0), b(0) {}

Spectrum::Spectrum(float r, float g, float b): r(r), g(g), b(b) {}

bool Spectrum::IsBlack() {
    return (r == 0.0f) && (g == 0.0f) && (b == 0.0f);
}

bool Spectrum::HasNeg() {
    return (r < 0.0f) || (g < 0.0f) || (b < 0.0f);
}

bool Spectrum::HasInf() {
    return isinf(r) || isinf(g) || isinf(b);
}

bool Spectrum::HasNaN() {
    return isnan(r) || isnan(g) || isnan(b);
}

Spectrum Spectrum::operator+(Spectrum s) {
    return Spectrum(r + s.r, g + s.g, b + s.b);
}

Spectrum Spectrum::operator-(Spectrum s) {
    return Spectrum(r - s.r, g - s.g, b - s.b);
}

Spectrum Spectrum::operator*(Spectrum s) {
    return Spectrum(r * s.r, g * s.g, b * s.b);
}

Spectrum Spectrum::operator*(float n) {
    return Spectrum(r * n, g * n, b * n);
}

Spectrum Spectrum::operator/(Spectrum s) {
    return Spectrum(r / s.r, g / s.g, b / s.b);
}

Spectrum Spectrum::operator/(float n) {
    return Spectrum(r / n, g / n, b / n);
}

float Spectrum::operator[](unsigned int i) {
    switch (i) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        default:
            cout << "[Error] Spectrum: Index out of range." << endl;
            exit(-1);
    }
}

Spectrum & Spectrum::operator+=(Spectrum s) {
    r += s.r;
    g += s.g;
    b += s.b;
    return *this;
}

Spectrum & Spectrum::operator-=(Spectrum s) {
    r -= s.r;
    g -= s.g;
    b -= s.b;
    return *this;
}

Spectrum & Spectrum::operator*=(Spectrum s) {
    r *= s.r;
    g *= s.g;
    b *= s.b;
    return *this;
}

Spectrum & Spectrum::operator*=(float n) {
    r *= n;
    g *= n;
    b *= n;
    return *this;
}

Spectrum & Spectrum::operator/=(Spectrum s) {
    r /= s.r;
    g /= s.g;
    b /= s.b;
    return *this;
}

Spectrum & Spectrum::operator/=(float n) {
    r /= n;
    g /= n;
    b /= n;
    return *this;
}
