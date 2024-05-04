#version 330 core

out vec4 fragColor;

in vec2 uv;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, uv);
}