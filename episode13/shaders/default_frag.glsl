#version 410 core

out vec4 color;
in vec3 FragColor;
void main() {
    color = vec4(FragColor, 1.0);
}
