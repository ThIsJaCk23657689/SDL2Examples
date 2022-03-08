#version 330 core

#define NUM_LIGHTS 8

out vec4 FragColor;

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

uniform float shininess;
uniform vec3 objectColor;
uniform sampler2D diffuse_texture;

uniform vec3 viewPos;
uniform bool useLighting;
uniform bool useBlinnPhong;
uniform bool useTexture;

uniform Light lights[NUM_LIGHTS];

vec3 CalcLight(Light light, vec3 normal, vec3 viewDir, vec3 color);

void main() {

    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.Normal);

    vec3 final_color = (useTexture) ? texture(diffuse_texture, fs_in.TexCoord).rgb : objectColor;

    // 計算光照
    vec3 illumination = vec3(0.0f);
    if (useLighting) {
        for (int i = 0; i < NUM_LIGHTS; i++ ) {
            if (lights[i].enable) {
                illumination += CalcLight(lights[i], norm, viewDir, final_color);
            }
        }
    }

    FragColor = vec4(illumination, 1.0f);
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
