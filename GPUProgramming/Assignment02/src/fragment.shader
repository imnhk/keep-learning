#version 410 core

in vec3 gPos;
in vec3 gNormal;
in float gNdotv;

flat in int gIsEdge;

uniform vec3 objColor = vec3(1.0, 1.0, 1.0);
uniform vec4 lineColor = vec4(0, 0, 0, 1.0);

layout(location = 0) out vec4 fragColor;

vec4 contour() {
	return vec4(objColor * gNdotv, 1.0);
}

void main()
{
	if (gIsEdge == 1) {
		fragColor = lineColor;

	} else {
		fragColor = contour();
	}
}