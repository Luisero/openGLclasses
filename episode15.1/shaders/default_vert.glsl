#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 FragColor;
void main()
{
    FragColor = color;
    gl_Position = vec4(position, 1.0);
}
