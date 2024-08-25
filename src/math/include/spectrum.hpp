// spectrum.hpp

#pragma once

class Spectrum {
    public:
        Spectrum();
        Spectrum(float r, float g, float b);
        float r, g, b;

        bool IsBlack();
        bool HasNeg();
        bool HasInf();
        bool HasNaN();

        Spectrum operator+(Spectrum s);
        Spectrum operator-(Spectrum s);
        Spectrum operator*(Spectrum s);
        Spectrum operator*(float n);
        Spectrum operator/(Spectrum s);
        Spectrum operator/(float n);
        float    operator[](unsigned int i);

        Spectrum & operator+=(Spectrum s);
        Spectrum & operator-=(Spectrum s);
        Spectrum & operator*=(Spectrum s);
        Spectrum & operator*=(float n);
        Spectrum & operator/=(Spectrum s);
        Spectrum & operator/=(float n);
};
