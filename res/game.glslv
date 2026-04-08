#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in int aTexLayer;

out vec2 TexCoord;
flat out int TexLayer;

uniform mat4 projview;
uniform float u_time;
uniform float u_amplitude;
uniform float u_speed;

void main() {
    vec3 pos = aPos;
    if (aTexLayer == 13 || aTexLayer == 15) {
        float scaleY = 1 + sin(u_time * u_speed) * u_amplitude;
        pos.y *= scaleY;
    }
    gl_Position = projview * vec4(pos, 1.0);
    TexCoord = aTexCoord;
    TexLayer = aTexLayer;
}
