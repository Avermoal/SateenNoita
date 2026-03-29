#version 460 core
in vec2 TexCoord;
flat in int TexLayer;

uniform sampler2DArray interfaceTextureArray;
out vec4 FragColor;

void main() {
    FragColor = texture(interfaceTextureArray, vec3(TexCoord, TexLayer));
}
