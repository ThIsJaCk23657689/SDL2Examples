#version 330 core

out vec4 FragColor;

uniform vec4 objectColor;

void main() {
    FragColor = vec4(objectColor);
}