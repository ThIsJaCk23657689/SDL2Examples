#version 330 core

out vec4 FragColor;

struct Light {
    vec3 ambient;
};

uniform vec3 objectColor;
uniform Light lights[1];

void main() {

    vec3 final_color = objectColor * lights[0].ambient;

    FragColor = vec4(final_color, 1.0f);
}