#version 410 core

uniform vec3 cameraPos = vec3(0, 0, 10);
uniform vec3 lightPos = vec3(10, 10, 10);
uniform vec3 objectColor = vec3(0.8, 0.8, 0.8);
uniform vec3 lightColor = vec3(0.9, 0.8, 0.7);
uniform vec3 ambientColor = vec3(0.2, 0.2, 0.2);
uniform float shininess = 1;

in vec3 normal;
in vec3 fragPos;

out vec4 out_Color;

void main(void)
{
	vec3 N = normalize(normal); // Normalized normal :)
	vec3 L = normalize(lightPos - fragPos); // vector to light
	vec3 V = normalize(cameraPos - fragPos); // vector to camera

	// diffuse
	float diffuse = clamp(dot(N, L), 0, 1);
	vec3 diffuseColor = diffuse * lightColor;

	// specular
	vec3 R = reflect(-L, normal);
	float specular = pow(clamp(dot(V, R), 0, 1), 32);
	vec3 specularColor = shininess * specular * lightColor;

	// result color
	vec3 I = objectColor * (diffuseColor + ambientColor + specularColor);

	// gamma correction
	float gamma = 2.2;
	I = pow(I, vec3(1.0 / gamma));

	out_Color = vec4(I, 1);
}
