#pragma once

void render(GLFWwindow* window);
void initialize();

// Callback 선언. 마우스 입력 회전을 위해 추가
void cursorCallback(GLFWwindow* win, double x, double y);
void buttonCallback(GLFWwindow* win, int btn, int action, int mods);
void scrollCallback(GLFWwindow* win, double xOffset, double yOffset);

const int PI = 3.141592;

// 카메라의 위치, 방향, FOV
struct Camera {
	const float FOV_MIN = 20, FOV_MAX = 100, DIST = 3;
	
	float yaw = 0, pitch = 0;
	float fov = 50;
	double lastX = 0, lastY = 0;

	glm::vec3 pos = glm::vec3(0.0, 0.0, DIST);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 look = glm::vec3(0.0, 0.0, 0.0);
};