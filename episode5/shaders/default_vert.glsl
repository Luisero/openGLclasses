#version 410 core

layout(location = 0) in vec3 position;
out vec3 aPos;
void main()
{
    aPos = vec3(position.xyz);
    gl_Position = vec4(position, 1.0);
}
