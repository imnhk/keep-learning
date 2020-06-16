#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 pos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int pass;

void main()
{
	if (pass == 2 || pass == 3) {
		texCoord = aTexCoord;
		pos = aPos;
		gl_Position = vec4(aPos.xy, 0.0, 1.0);
	}
	else {
		pos = aPos;
		normal = aNormal;
		texCoord = aTexCoord;
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}

}