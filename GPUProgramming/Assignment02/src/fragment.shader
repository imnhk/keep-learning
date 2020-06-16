#version 330 core

in vec3 pos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 cameraPos;
uniform vec3 objColor = vec3(1.0, 1.0, 1.0);
uniform int pass;
uniform float edgeThreshold = 0.5;

layout(location = 0) out vec4 fragColor;
uniform sampler2D renderedTex;

vec4 contour() {
	// NdotV
	vec3 V = normalize(cameraPos - pos);
	vec3 N = normalize(normal);

	float NdotV = dot(N, V);
	return vec4(objColor * NdotV, 1.0);
}

vec4 pass1() {
	// Edge

	return vec4(0, 0, 0, 1.0);
}

void main()
{
	if (pass == 0)
		fragColor = contour();
	else if (pass == 1)
		fragColor = pass1();


}