#version 330 core
layout (location = 0) out vec4 FragColor;

#define NUM_LIGHTS 8

in VS_OUT {
    vec3 FragPos;
    vec3 TexCoord;
} fs_in;

uniform sampler3D perlinNoiseTexture;
uniform vec3 g_ObjectColor;
uniform vec3 g_ObjectPosition;
uniform vec3 g_ObjectSize;
uniform int g_SampleCount;
uniform float g_Absorbance;

uniform vec3 g_ViewPos;

vec4 CalcFog(vec4 color);
float CalcTransmittance(float d);
vec2 rayBoxDst(vec3 BBMin, vec3 BBMax, vec3 rayOrigin, vec3 rayDirection);
float sampleDensity(vec3 position);

void main() {
    // Initialize
    vec4 result = vec4(0.0f);
    float transmittance = 1.0f;

    vec3 rayOrigin = g_ViewPos;
    vec3 rayDirection = normalize(fs_in.FragPos - rayOrigin);

    vec3 halfSize = g_ObjectSize / 2.0f;
    vec3 BBMin = g_ObjectPosition - halfSize;
    vec3 BBMax = g_ObjectPosition + halfSize;
    vec2 rayBoxInfo = rayBoxDst(BBMin, BBMax, rayOrigin, rayDirection);
    float dstToBox = rayBoxInfo.x;
    float dstInsideBox = rayBoxInfo.y;

    float stepSize = dstInsideBox / g_SampleCount;
    vec3 currentPos = fs_in.FragPos;
    float totalDensity = 0.0f;

    for (int march = 0; march < 64; march++) {
        currentPos = rayOrigin + rayDirection * (dstToBox + march * stepSize);

        vec3 samplePos = fs_in.TexCoord;
        vec3 sampleRayDir = vec3(1.0f);
        sampleRayDir = rayDirection / g_ObjectSize;
        samplePos = samplePos + sampleRayDir * march * stepSize;

        totalDensity += sampleDensity(samplePos) * stepSize;

        if (length(currentPos) > dstInsideBox) {
            break;
        }
    }

    FragColor = vec4(vec3(texture(perlinNoiseTexture,fs_in.TexCoord)), 1.0f);

//    transmittance = CalcTransmittance(totalDensity);
//    float alpha = 1.0f - transmittance;
//    result = vec4(g_ObjectColor * alpha, alpha);

//    if (result.a < 0.01f) {
//        discard;
//    }

    // 正常模式下，計算濃霧效果
//    FragColor = result;
}

float CalcTransmittance(float d) {
    return exp(-d * g_Absorbance);
}

float sampleDensity(vec3 position) {
    float density = texture(perlinNoiseTexture, position).r;
    return density;
}

vec2 rayBoxDst(vec3 BBMin, vec3 BBMax, vec3 rayOrigin, vec3 rayDirection)
{
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