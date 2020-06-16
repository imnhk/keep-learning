#version 330 core

layout(triangles) in;
layout(points, max_vertices = 100) out;

in vec3 vPos[];
in vec3 vNormal[];
in vec2 texCoord[];

uniform int pass;

out vec3 fColor;

void main() {
	if (pass == 0) {
		gl_Position = gl_in[0].gl_Position;
	
	}
	else if (pass == 1) {
		fColor = vec3(0, 0, 0);

		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		EndPrimitive();
	}

}