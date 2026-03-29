#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aTexLayer;

out vec2 TexCoord;
flat out int TexLayer;
uniform mat4 projview;

void main(void)
{
    gl_Position = projview*vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
    TexLayer = aTexLayer;
}
