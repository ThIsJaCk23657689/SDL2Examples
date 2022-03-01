#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
    FragColor = vec4(clamp(objectColor, 0.0f, 1.0f), 1.0f);
}