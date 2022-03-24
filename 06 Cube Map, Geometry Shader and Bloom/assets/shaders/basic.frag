#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    vec3 FragPos;
} fs_in;

struct Fog {
    vec3 color;
    int mode;
    float density;
    float gradient;
    float start;
    float end;
    bool enable;
};

uniform vec3 objectColor;

uniform vec3 viewPos;
uniform Fog fog;

vec4 CalcFog(vec4 color);

void main() {
    // 計算濃霧
    vec4 final_color = CalcFog(vec4(objectColor, 1.0f));

    FragColor = final_color;
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0f) {
        BrightColor = vec4(FragColor.rgb, 1.0f);
    } else {
        BrightColor = vec4(0, 0, 0, 1.0f);
    }
}

vec4 CalcFog(vec4 color) {
    vec4 final_color = vec4(0.0f);
    float distance = length(viewPos - fs_in.FragPos);
    float fog_factor = 0.0f;
    if (fog.enable) {
        switch (fog.mode) {
            case 0:
            fog_factor = clamp((fog.end - distance) / (fog.end - fog.start), 0.0f, 1.0f);
            break;
            case 1:
            fog_factor = clamp(1.0 / exp(pow(fog.density * distance, fog.gradient)), 0.0f, 1.0f);
            break;
            case 2:
            fog_factor = clamp(1.0 / exp(pow(fog.density * distance * distance, fog.gradient)), 0.0f, 1.0f);
            break;
        }
        final_color = mix(vec4(fog.color, 1.0f), color, fog_factor);
    } else {
        final_color = color;
    }

    return final_color;
}