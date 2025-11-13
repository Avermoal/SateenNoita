#version 460 core
layout(location = 0) in vec3 vertexpos;
layout(location = 1) in vec3 vertexcol;

out vec3 color;

void main() {
	color = vertexcol;
	gl_Position = vec4(vertexpos)
}
