#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vPos;
out vec3 vNormal;
out float vNdotv;

// uniform mat4 model;
uniform mat4 view;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projection;
uniform vec3 cameraPos;

// uniform int pass;

void main()
{

	vPos = vec3(projection * view * vec4(aPos, 1.0));
	vNormal = normalMatrix * aNormal;


	// N dot V를 vertex에서 계산
	vec3 V = normalize(cameraPos - aPos);
	vec3 N = normalize(aNormal);
	vNdotv = dot(N, V);

	gl_Position = projection * view * vec4(aPos, 1.0);


}