#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 10) out;

in vec3 vPos[];
in vec3 vNormal[];
in float vNdotv[];

out vec3 gPos;
out vec3 gNormal;
out float gNdotv;

uniform int pass;
uniform float halfWidth = 0.1;

void EmitEdge(vec3 p1, vec3 p2) {

	vec2 v = normalize(p2.xy - p1.xy);
	vec2 n = vec2(-v.y, v.x) * halfWidth;

	gl_Position = vec4(p1.xy + n, p1.z, 1.0);
	EmitVertex();
	gl_Position = vec4(p1.xy - n, p1.z, 1.0);
	EmitVertex();
	gl_Position = vec4(p2.xy + n, p2.z, 1.0);
	EmitVertex();
	gl_Position = vec4(p2.xy - n, p2.z, 1.0);
	EmitVertex();

	return;
}

void main() {

	// parameter 그대로 fragment에 전달
	if (pass == 0) {

		for (int i = 0; i < 3; i++)
		{
			gPos = vPos[i];
			gNormal = vNormal[i];
			gNdotv = vNdotv[i];

			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
	// 외곽선 추가
	else if (pass == 1) {

		float ndv[3] = { vNdotv[0], vNdotv[1], vNdotv[2] };

		// 세 vertex에서 NdotV의 방향이 모두 같으면 제외
		if (ndv[0] > 0 && ndv[1] > 0 && ndv[0] > 0)
			return;
		if (ndv[0] < 0 && ndv[1] < 0 && ndv[0] < 0)
			return;

		for (int i = 0; i < 3; i++)
		{
			gPos = vPos[i];
			gNormal = vNormal[i];
			gNdotv = vNdotv[i];

			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
		}

		// 보간으로 추가할 실루엣의 양 끝 위치를 구한다

		// 실루엣을 추가한다

		vec3 p1 = vPos[0];
		vec3 p2 = vPos[1];

		vec2 v = normalize(p2.xy - p1.xy);
		vec2 n = vec2(-v.y, v.x) * halfWidth;

		gl_Position = vec4(p1.xy + n, p1.z, 1.0);
		EmitVertex();
		gl_Position = vec4(p1.xy - n, p1.z, 1.0);
		EmitVertex();
		gl_Position = vec4(p2.xy + n, p2.z, 1.0);
		EmitVertex();
		gl_Position = vec4(p2.xy - n, p2.z, 1.0);
		EmitVertex();



		EndPrimitive();
	}

}