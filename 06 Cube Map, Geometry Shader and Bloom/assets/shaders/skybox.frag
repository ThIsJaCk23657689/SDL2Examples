#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float bloomThreshold;

void main() {
    vec4 color = texture(skybox, TexCoords);

    // 計算亮度有沒有超過 1.0f，用於泛光特效使用
    vec4 bright_color = vec4(0.0f);
    float brightness = dot(color.rgb * 5.0f, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > bloomThreshold) {
        bright_color = vec4(color.rgb, 1.0f);
    } else {
        bright_color = vec4(0, 0, 0, 1.0f);
    }
    BrightColor = bright_color;
    FragColor = color;
}