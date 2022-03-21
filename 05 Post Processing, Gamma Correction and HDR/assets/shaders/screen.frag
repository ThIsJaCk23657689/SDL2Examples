#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int screenMode;
uniform sampler2D screenTexture;

uniform bool useGamma;
uniform float gammaValue;

// uniform float exposure;

const float offset = 1.0f / 300.0f;

vec4 CalcKernel(float kernel[9]) {
    vec4 result_color = vec4(0.0f);
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0f, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(offset, 0.0f),
        vec2(-offset, -offset),
        vec2(0.0f, -offset),
        vec2(offset, -offset)
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 temp_color = vec3(0.0f);
    for(int i = 0; i < 9; i++) {
        temp_color += sampleTex[i] * kernel[i];
    }

    result_color = vec4(temp_color, 1.0f);
    return result_color;
}

void main() {


    // vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));
    // vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);

    // mapped = pow(mapped, vec3(1.0 / gamma));

    // FragColor = vec4(mapped, 1.0f);

    vec4 main_color = texture(screenTexture, TexCoords);
    switch (screenMode) {
        case 1:
            // 顏色相反，
            main_color = vec4(vec3(1.0 - main_color.rgb), 1.0f);
            break;
        case 2:
            // 灰階度，人眼對於綠色更加敏感，這樣計算會更加有物理精準效果（配上權重）。
            float average = 0.2126f * main_color.r + 0.7152 * main_color.g + 0.0722 * main_color.b;
            main_color = vec4(average, average, average, 1.0f);
            break;
        case 3:
            // 麻醉效果
            float kernel_Narc[9] = float[](
                -1, -1, -1,
                -1,  9, -1,
                -1, -1, -1
            );
            main_color = CalcKernel(kernel_Narc);
            break;
        case 4:
            // 模糊效果
            float kernel_blur[9] = float[](
                1.0f / 16f, 2.0f / 16.0f, 1.0f / 16.0f,
                2.0f / 16f, 4.0f / 16.0f, 2.0f / 16.0f,
                1.0f / 16f, 2.0f / 16.0f, 1.0f / 16.0f
            );
            main_color = CalcKernel(kernel_blur);
            break;
        case 5:
            // 邊緣偵測
            float kernel_ED[9] = float[](
                1, 1, 1,
                1, -8, 1,
                1, 1, 1
            );
            main_color = CalcKernel(kernel_ED);
            break;
    }

    if (useGamma) {
        main_color = vec4(pow(main_color.xyz, vec3(1.0 / gammaValue)), 1.0f);
    }

    FragColor = main_color;
}