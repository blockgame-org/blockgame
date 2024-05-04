#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inUV;

out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    uv = inUV;

    mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4(inPos, 1.0);
}