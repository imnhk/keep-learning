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

// Gaussian blur
const int PixOffset[5] = int[](0, 1, 2, 3, 4);
const float Weight[5] = float[](0.4, 0.25, 0.15, 0.1, 0.1);

vec4 contour() {
	// NdotV
	vec3 V = normalize(cameraPos - pos);
	vec3 N = normalize(normal);

	float NdotV = dot(N, V);
	return vec4(objColor * NdotV, 1.0);
}

vec4 pass1() {
	// Edge
	vec3 V = normalize(cameraPos - pos);
	vec3 N = normalize(normal);

	float NdotV = dot(N, V);
	if (NdotV < edgeThreshold)
		NdotV = 0;
	else
		NdotV = 1;

	return vec4(objColor * NdotV, 1.0);
}

vec4 pass2() {
	// Gaussian blur 1 (vertical)
	ivec2 pix = ivec2(gl_FragCoord.xy);
	vec4 sum = texelFetch(renderedTex, pix, 0) * Weight[0];
	
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, PixOffset[0])) * Weight[0];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, -PixOffset[0])) * Weight[0];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, PixOffset[1])) * Weight[1];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, -PixOffset[1])) * Weight[1];
	
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, PixOffset[2])) * Weight[2];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, -PixOffset[2])) * Weight[2];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, PixOffset[3])) * Weight[3];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, -PixOffset[3])) * Weight[3];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, PixOffset[4])) * Weight[4];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(0, -PixOffset[4])) * Weight[4];
	
	return sum;
}

vec4 pass3() {
	// Gaussian blur 2 (horizontal)
	ivec2 pix = ivec2(gl_FragCoord.xy);
	vec4 sum = texelFetch(renderedTex, pix, 0) * Weight[0];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(PixOffset[0], 0)) * Weight[0];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(-PixOffset[0], 0)) * Weight[0];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(PixOffset[1], 0)) * Weight[1];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(-PixOffset[1], 0)) * Weight[1];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(PixOffset[2], 0)) * Weight[2];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(-PixOffset[2], 0)) * Weight[2];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(PixOffset[3], 0)) * Weight[3];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(-PixOffset[3], 0)) * Weight[3];

	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(PixOffset[4], 0)) * Weight[4];
	sum += texelFetchOffset(renderedTex, pix, 0, ivec2(-PixOffset[4], 0)) * Weight[4];

	return sum;
}

void main()
{
	if (pass == 0)
		fragColor = contour();
	else if (pass == 1)
		fragColor = pass1();
	else if (pass == 2)
		fragColor = pass2();
	else if (pass == 3)
		fragColor = pass3();
}