#version 460 core
in vec2 TexCoord;
flat in int TexLayer;

uniform sampler2DArray fontTextureArray;
out vec4 FragColor;

void main()
{
    FragColor = texture(fontTextureArray, vec3(TexCoord, TexLayer));
}
