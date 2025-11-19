#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoord;

uniform mat4 projview;

void main(void)
{
    gl_Position = projview*vec4(aPos, 1.0f);
    FragPos = aPos;
    TexCoord = aTexCoord;
}
