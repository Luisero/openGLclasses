#version 410 core

out vec4 color;
in vec3 aPos;
void main() {
    color = vec4(aPos.x + 0.5, aPos.y + 0.5, aPos.z + 0.5, 1.0);
}
