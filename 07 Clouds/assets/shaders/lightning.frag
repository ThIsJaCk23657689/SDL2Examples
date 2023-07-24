#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

#define NUM_LIGHTS 8

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;

    bool enable;
    int caster;
};

struct Fog {
    vec3 color;
    int mode;
    float density;
    float gradient;
    float start;
    float end;
    bool enable;
};

uniform float shininess;
uniform vec3 objectColor;
uniform sampler2D diffuse_texture;
uniform samplerCube skybox;

uniform vec3 viewPos;
uniform bool useLighting;
uniform bool useBlinnPhong;
uniform bool useTexture;
uniform bool emissionTexture;

uniform float bloomThreshold;

uniform Light lights[NUM_LIGHTS];
uniform Fog fog;

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir, vec3 color);
vec4 CalcFog(vec4 color);

void main() {

    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec4 final_frag_color = (useTexture) ? texture(diffuse_texture, fs_in.TexCoord) : vec4(objectColor, 1.0f);

    // 計算反射
    // vec3 R = reflect(-viewDir, norm);
    // final_frag_color += vec4(texture(skybox, R).rgb * 0.5, 0.0f);

    // 計算光照
    vec3 illumination = vec3(0.0f);
    if (useLighting) {
        for (int i = 0; i < NUM_LIGHTS; i++ ) {
            if (emissionTexture) {
                // 發現如果該貼圖具有 emission 特性，就不用算光照直接跳出迴圈
                illumination = final_frag_color.rgb * 10.0f;
                break;
            }
            if (lights[i].enable) {
                illumination += CalcLight(lights[i], norm, viewDir, final_frag_color.rgb);
            }
        }
    }

    // 去透明
    if (final_frag_color.a < 0.1f) {
        discard;
    }

    // 計算亮度有沒有超過 1.0f，用於泛光特效使用
    vec4 bright_color = vec4(0.0f);
    float brightness = dot(illumination, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > bloomThreshold) {
        bright_color = vec4(illumination, 1.0f);
    } else {
        bright_color = vec4(0, 0, 0, 1.0f);
    }
    // 再計算濃霧效果
    bright_color = CalcFog(vec4(bright_color.rgb, 1.0f));
    BrightColor = bright_color;

    // 正常模式下，計算濃霧效果
    vec4 final_color = CalcFog(vec4(illumination, final_frag_color.a));
    FragColor = final_color;
}

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir, vec3 color) {

    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    vec3 lightDir = vec3(0.0f);
    if (light.caster == 0) {
        // Directional Light
        lightDir = normalize(-light.direction);
    } else {
        lightDir = normalize(light.position - fs_in.FragPos);
    }

    float diff = max(dot(normal, lightDir), 0.0f);

    float spec = 0.0f;
    if (useBlinnPhong) {
        vec3 halfway = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfway), 0.0f), shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);
    }

    ambient = light.ambient * color;
    diffuse = light.diffuse * diff * color;
    specular = light.specular * spec * color;

    // Attenuation
    if (light.caster != 0) {
        float distance = length(light.position - fs_in.FragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        if (light.caster == 2) {
            // Spotlight
            float intensity = 0.0f;
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = light.cutoff - light.outerCutoff;
            intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

            ambient *= intensity;
            diffuse *= intensity;
            specular *= intensity;
        }
    }

    return ambient + diffuse + specular;
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