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
uniform float halfWidth = 0.005;

void EmitEdge(vec3 p0, vec3 p1) {

	vec2 v = normalize(p1.xy - p0.xy);
	vec2 n = vec2(-v.y, v.x) * halfWidth;
	vec2 ext = (p1.xy - p0.xy) * 0.25;

	gl_Position = vec4(p0.xy + n - ext, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(p0.xy - n - ext, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(p1.xy + n + ext, 0.0, 1.0);
	EmitVertex();
	gl_Position = vec4(p1.xy - n + ext, 0.0, 1.0);
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
		if (ndv[0] < 0 && ndv[1] < 0 && ndv[2] < 0)
			return;
		if (ndv[0] > 0 && ndv[1] > 0 && ndv[2] > 0)
			return;
		
		// 보간으로 추가할 실루엣의 양 끝 위치를 구한다
		vec3 p0, p1;
		if (ndv[0] > 0 && ndv[1] < 0 && ndv[2] < 0) {
			p0 = mix(vPos[0], vPos[1], ndv[0] / (ndv[0] - ndv[1]));
			p1 = mix(vPos[0], vPos[2], ndv[0] / (ndv[0] - ndv[2]));
		}
		else if (ndv[0] < 0 && ndv[1] > 0 && ndv[2] < 0) {
			p0 = mix(vPos[1], vPos[0], ndv[1] / (ndv[1] - ndv[0]));
			p1 = mix(vPos[1], vPos[2], ndv[1] / (ndv[1] - ndv[2]));
		}
		else if (ndv[0] < 0 && ndv[1] < 0 && ndv[2] > 0) {
			p0 = mix(vPos[2], vPos[0], ndv[2] / (ndv[2] - ndv[0]));
			p1 = mix(vPos[2], vPos[1], ndv[2] / (ndv[2] - ndv[1]));
		}

		if (ndv[0] < 0 && ndv[1] > 0 && ndv[2] > 0) {
			p0 = mix(vPos[0], vPos[1], -ndv[0] / (ndv[1] - ndv[0]));
			p1 = mix(vPos[0], vPos[2], -ndv[0] / (ndv[2] - ndv[0]));
		}
		else if (ndv[0] > 0 && ndv[1] < 0 && ndv[2] > 0) {
			p0 = mix(vPos[1], vPos[0], -ndv[1] / (ndv[0] - ndv[1]));
			p1 = mix(vPos[1], vPos[2], -ndv[1] / (ndv[2] - ndv[1]));
		}
		else if (ndv[0] > 0 && ndv[1] < 0 && ndv[2] > 0) {
			p0 = mix(vPos[2], vPos[0], -ndv[2] / (ndv[0] - ndv[2]));
			p1 = mix(vPos[2], vPos[1], -ndv[2] / (ndv[1] - ndv[2]));
		}

		// 실루엣을 추가한다
		EmitEdge(p0, p1);

	}

}