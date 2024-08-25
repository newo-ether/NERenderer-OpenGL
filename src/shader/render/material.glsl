// material.glsl

#include "objectptr.glsl"

struct Material {
    // Pack 1
    float type;
    vec3 kd;

    // Pack 2
    float isGlow;
    vec3 glowSpectrum;
};

Material MaterialInit(ObjectPtr objPtr) {
    vec4 materialData[2];
    float index = float(objPtr.index);

    materialData[0] = texture1D(objectList, (float(index) + 3.0f + 0.5f) / float(packNum));
    materialData[1] = texture1D(objectList, (float(index) + 4.0f + 0.5f) / float(packNum));

    Material material;
    material.type = materialData[0].x;
    material.kd = materialData[0].yzw;
    material.isGlow = materialData[1].x;
    material.glowSpectrum = materialData[1].yzw;

    return material;
}

Material MaterialInitEmpty() {
    Material material;
    material.type = 0.0f;
    material.kd = vec3(0.0f, 0.0f, 0.0f);
    material.isGlow = 0.0f;
    material.glowSpectrum = vec3(0.0f, 0.0f, 0.0f);

    return material;
}

bool MaterialIsGlow(Material material) {
    return material.isGlow == 0.0f ? false : true;
}

vec3 MaterialGetGlowSpectrum(Material material) {
    return material.glowSpectrum;
}

vec3 MaterialBSDF(Material material, vec3 wi, vec3 wo, vec3 normal) {
    if (material.type == 0.0f) {
        return material.kd * INVPI;
    }
    else {
        return vec3(0.0f, 0.0f, 0.0f);
    }
}
