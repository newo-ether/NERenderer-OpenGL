// material.cpp

#include "include/material.hpp"

#include "../math/include/spectrum.hpp"

MaterialData Material::GetMaterialData() const {
    return materialData;
}

Lambertian::Lambertian(Spectrum s, Spectrum glow) {
    materialData.type = 0.0f;

    materialData.kd[0] = s[0];
    materialData.kd[1] = s[1];
    materialData.kd[2] = s[2];

    materialData.isGlow = !glow.IsBlack();

    materialData.glowSpectrum[0] = glow[0];
    materialData.glowSpectrum[1] = glow[1];
    materialData.glowSpectrum[2] = glow[2];
}
