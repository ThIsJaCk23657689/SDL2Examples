#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aTexCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 TexCoord;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPosition, 1.0f));
    vs_out.TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}