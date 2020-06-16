#version 330 core

in vec3 gPos;
in vec3 gNormal;
in float gNdotv;

uniform vec3 objColor = vec3(1.0, 1.0, 1.0);
uniform int pass;

layout(location = 0) out vec4 fragColor;

vec4 contour() {
	return vec4(objColor * gNdotv, 1.0);
}

vec4 silhouettes() {
	return vec4(0, 0, 0, 1.0);
}

void main()
{
	if (pass == 0)
		fragColor = contour();
	else if (pass == 1)
		fragColor = silhouettes();
}