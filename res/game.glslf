#version 460 core
in vec2 TexCoord;
flat in int TexLayer;

uniform sampler2DArray tileTextureArray;
out vec4 FragColor;

void main() {
    FragColor = texture(tileTextureArray, vec3(TexCoord, TexLayer));
}

