// random.glsl

uniform usampler2D randomSeed;
uniform uint extraSeed;

const uint LCG_A = uint(16807);
const uint LCG_C = uint(0);
const uint LCG_M = uint(2147483647);

uint seed;

void RandomInit(vec2 screenIndex) {
    seed = texture(randomSeed, screenIndex / vec2(screenGeometry)).x;
    seed = (seed + extraSeed) % LCG_M;
}

uint RandomUint() {
    seed = (seed * LCG_A + LCG_C) % LCG_M;
    return seed;
}

float Random01() {
    return float(RandomUint() - uint(1)) / float(LCG_M - uint(1));
}

float Random11() {
    return Random01() * 2.0f - 1.0f;
}

vec3 RandomSphereDir() {
    float z = Random11();
    float r = sqrt(1.0f - z * z);
    float phi = Random01() * TWOPI;
    return vec3(r * cos(phi), r * sin(phi), z);
}

vec3 RandomHemisphereDir(vec3 normal) {
    vec3 randDir = RandomSphereDir();
    if (dot(normal, randDir) < 0) {
        randDir = -randDir;
    }
    return randDir;
}
