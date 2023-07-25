#version 330 core
layout (location = 0) out vec4 FragColor;

#define NUM_LIGHTS 8

in VS_OUT {
    vec3 FragPos;
    vec3 TexCoord;
} fs_in;

struct Light {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    bool enable;
};

uniform sampler3D perlinNoiseTexture;
uniform vec3 g_ObjectColor;
uniform vec3 g_ObjectPosition;
uniform vec3 g_ObjectSize;
uniform int g_SampleCount;
uniform float g_Absorbance;

uniform vec3 g_ViewPos;

uniform vec3 g_NoiseOffset;
uniform float g_NoiseScale;
uniform float g_DensityThreshold;
uniform float g_DensityMultiplier;

uniform Light g_Light;
uniform float g_DarknessThreshold;
uniform float g_PhaseParameter1;
uniform float g_PhaseParameter2;
uniform bool g_UseAnisotropic;

vec2 rayBoxDst(vec3 BBMin, vec3 BBMax, vec3 rayOrigin, vec3 rayDirection);
float sampleDensity(vec3 position);
float lightMarch(vec3 BBMin, vec3 BBMax, vec3 position);
float henyeyGreenstein(float g, float costheta);
float TwoLobeHGPhase(float alpha, float costheta);
float Beers(float density);
float BeersPowder(float density);
float MultipleOctaveScattering(float density, float mu);

void main() {
    // Initialize
    vec3 rayOrigin = g_ViewPos;
    vec3 rayDirection = normalize(fs_in.FragPos - rayOrigin);

    vec3 halfSize = g_ObjectSize / 2.0f;
    vec3 BBMin = g_ObjectPosition - halfSize;
    vec3 BBMax = g_ObjectPosition + halfSize;

    // 求出射線與 BB 的交點
    vec2 rayBoxInfo = rayBoxDst(BBMin, BBMax, rayOrigin, rayDirection);
    float dstToBox = rayBoxInfo.x;
    float dstInsideBox = rayBoxInfo.y;

    // ray march 起始位置
    vec3 startPos = rayOrigin + rayDirection * dstToBox;

    // 每次 sample 間距長度
    float stepSize = dstInsideBox / g_SampleCount;

    // 總密度
    vec4 result = vec4(0.0f);
    vec3 lightEngergy = vec3(0.0f);
    float transmittance = 1.0f;
    float totalDensity = 0.0f;

    for (int march = 0; march <= g_SampleCount; march++) {
        float deltaX = march * stepSize;
        vec3 currentPos = startPos + rayDirection * deltaX;
        if (deltaX > dstInsideBox) {
            break;
        }

        vec3 samplePos = fs_in.TexCoord;
        vec3 sampleRayDir = vec3(1.0f);
        sampleRayDir = rayDirection / g_ObjectSize;
        samplePos = samplePos + sampleRayDir * deltaX;

        float density = sampleDensity(samplePos);

        if (density > 0) {
            float phaseValue = 1.0f;
            if (g_UseAnisotropic) {
                // Anistropic scattering
                vec3 dirToLight = normalize(g_Light.position - currentPos);
                float scatteringAngle = dot(rayDirection, dirToLight);
                phaseValue = TwoLobeHGPhase(0.5f, scatteringAngle);
            }

            float lightTransmittance = (g_Light.enable) ? lightMarch(BBMin, BBMax, currentPos) : 0.0f;
            lightEngergy += density * stepSize * transmittance * lightTransmittance * phaseValue;

            transmittance *= Beers(density * stepSize);
            if (transmittance < 0.01f) {
                break;
            }
        }
    }

    // transmittance = CalcTransmittance(totalDensity);
    // float alpha = 1.0f - transmittance;
    // result = vec4(g_ObjectColor * alpha, alpha);

    vec3 cloudColor = lightEngergy * g_Light.color;
    float alpha = 1.0f - transmittance;
    result = vec4(g_ObjectColor * alpha * cloudColor, alpha);

    if (result.a < 0.01f) {
        discard;
    }
    FragColor = result;
}

float sampleDensity(vec3 position) {
    vec3 uvw = position * g_NoiseScale + g_NoiseOffset;
    float density = texture(perlinNoiseTexture, uvw).r;
    density = max(0, density - g_DensityThreshold) * g_DensityMultiplier;
    return density;
}

vec2 rayBoxDst(vec3 BBMin, vec3 BBMax, vec3 rayOrigin, vec3 rayDirection) {
    vec3 t0 = (BBMin - rayOrigin) / rayDirection;
    vec3 t1 = (BBMax - rayOrigin) / rayDirection;

    vec3 tMin = min(t0, t1);
    vec3 tMax = max(t0, t1);

    float dstA = max(max(tMin.x, tMin.y), tMin.z);
    float dstB = min(min(tMax.x, tMax.y), tMax.z);

    float dstToBox = max(0.0f, dstA);
    float dstInBox = max(0.0f, dstB - dstToBox);

    return vec2(dstToBox, dstInBox);
}

float lightMarch(vec3 BBMin, vec3 BBMax, vec3 position) {
    vec3 dirToLight = normalize(g_Light.position - position);
    float dstInsideBox = rayBoxDst(BBMin, BBMax, position, dirToLight).y;

    // 求區間
    float stepSize = dstInsideBox / 8;

    // 總密度
    float totalDensity = 0;

    for (int march = 0; march <= 8; ++march) {
        float deltaX = march * stepSize;
        vec3 currentPos = position + dirToLight * deltaX;
        if (deltaX > dstInsideBox) {
            break;
        }

        vec3 samplePos = fs_in.TexCoord;
        vec3 sampleRayDir = vec3(1.0f);
        sampleRayDir = dirToLight / g_ObjectSize;
        samplePos = samplePos + sampleRayDir * deltaX;
        float density = sampleDensity(samplePos);

        totalDensity += density * stepSize;
    }

    float transmittance = BeersPowder(totalDensity);
    return g_DarknessThreshold + transmittance * (1 - g_DarknessThreshold);
}

float henyeyGreenstein(float g, float costheta) {
    float g2 = g * g;
    return (1.0 / (4.0 * 3.14159265359)) * ((1.0 - g2)/(pow(1.0 + g2 - 2.0 * g * costheta, 1.5)));
}

float Beers(float density) {
    return exp(-density * g_Absorbance);
}

float BeersPowder(float density) {
    return  Beers(density) * (1 - exp(-2 * density));
}

float TwoLobeHGPhase(float alpha, float costheta) {
    return (1 - alpha) * henyeyGreenstein(g_PhaseParameter1, costheta) + alpha * henyeyGreenstein(g_PhaseParameter2, costheta);
}

float MultipleOctaveScattering(float density, float mu) {
    float attenuation = 0.2;
    float contribution = 0.4;
    float phaseAttenuation = 0.1;

    const float scatteringOctaves = 4.0;

    float a = 1.0;
    float b = 1.0;
    float c = 1.0;
    float g = 0.85;

    float luminance = 0.0;
    for (float i = 0.0; i < scatteringOctaves; i++) {
        float phaseFunction = TwoLobeHGPhase(0.3 * c, mu);
        float beers = BeersPowder(-density * a);

        luminance += b * phaseFunction * beers;

        a *= attenuation;
        b *= contribution;
        c *= (1.0 - phaseAttenuation);
    }
    return luminance;
}