#version 460 core

in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;

void main(void)
{
    FragColor = texture(uTexture, TexCoord);
}
