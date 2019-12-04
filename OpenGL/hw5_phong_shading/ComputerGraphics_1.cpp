//
//  main.cpp
//
//  Created by Hyun Joon Shin on 13/03/2018.
//  Copyright © 2018 Hyun Joon Shin. All rights reserved.
//


#ifdef WIN32
#include <Windows.h>
#define GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#elif defined __APPLE__
#pragma clang diagnostic ignored "-Wdocumentation"
#include <OpenGL/gl3.h>
#endif

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef WIN32
#pragma comment (lib, "glew32s")
#pragma comment (lib, "opengl32")
#pragma comment (lib, "glfw3")
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "j3a.hpp"
#include "toys.h"
using namespace glm;

#include <glm/gtc/matrix_transform.hpp>

void render(GLFWwindow* window);
void initialize();

GLuint triangleVBO;
GLuint normalVBO; // phong shading을 위해 추가
GLuint vertexArrayID;
GLuint indexVBOID;
Program program;

// 마우스 입력을 통한 카메라 회전을 위한 Callback
void cursorCallback(GLFWwindow* win, double x, double y);
void buttonCallback(GLFWwindow* win, int btn, int action, int mods);
void scrollCallback(GLFWwindow* win, double xOffset, double yOffset);

int main(int argc, const char * argv[]) {

	if (!glfwInit())        exit(EXIT_FAILURE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Namhyeon Kim 201521053", NULL, NULL);
	glfwMakeContextCurrent(window);

	//callback 사용 설정
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, buttonCallback);
	glfwSetScrollCallback(window, scrollCallback);

	glfwSwapInterval(1);

#ifdef WIN32
	glewInit();
#endif

	initialize();
	while (!glfwWindowShouldClose(window)) {
		render(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
#ifdef WIN32
#define PATH "../../ComputerGraphics_4/ComputerGraphics_1/"
#else
#define PATH ""
#endif

void initialize() {
	loadJ3A(PATH"bust.j3a");
	program.loadShaders("shader.vert", "shader.frag");

	// Vertex Array 생성, 활성화
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// vertex의 위치 정보를 저장하는 buffer
	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec3), vertices[0], GL_STATIC_DRAW);
	// buffer의 데이터로 수정할 vertex attribute의 index 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(0);
	
	// vertex의 normal 정보를 저장하는 buffer
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, nVertices[0] * sizeof(vec3), normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(1);

	// Index buffer 생성
	glGenBuffers(1, &indexVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
}

// 카메라의 위치, 방향, FOV
struct Camera {
	const float fov_min = 20, fov_max = 100, dist = 3;

	float yaw = 0, pitch = 0;
	float fov = 50;
	double lastX = 0, lastY = 0;

	vec3 pos = { 0, 0, dist };
	vec3 up = { 0, 1, 0 };
	vec3 look = { 0, 0, 0 };
};
Camera cam;


// Callback 함수들 정의
const float PI = 3.1415926535;
void cursorCallback(GLFWwindow* win, double x, double y) {
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1)) {
		cam.yaw -= (x - cam.lastX) / 200;
		cam.pitch = clamp<float>(cam.pitch - ((y - cam.lastY) / 200), PI / -2 + 0.1f, PI / 2 - 0.1f);
		cam.lastY = y;
		cam.lastX = x;
	}
}

void buttonCallback(GLFWwindow* win, int btn, int action, int mods) {
	if (btn == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		glfwGetCursorPos(win, &cam.lastX, &cam.lastY);
}

void scrollCallback(GLFWwindow* win, double xOffset, double yOffset) {
	cam.fov = clamp<float>(cam.fov -= yOffset * 2, cam.fov_min, cam.fov_max);
}

void render(GLFWwindow* window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.7, 0.7, 0.7, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program.programID);
	glBindVertexArray(vertexArrayID);
	glEnable(GL_DEPTH_TEST);

	// Camera 구조체 값 갱신
	cam.pos = 
		rotate(cam.yaw, vec3(0, 1, 0)) * 
		rotate(cam.pitch, vec3(1, 0, 0)) * 
		vec4(0, 0, cam.dist, 0);

	// glm::lookAt을 사용해서 카메라의 위치와 각도를 설정
	GLuint viewMatLoc = glGetUniformLocation(program.programID, "viewMat");
	mat4 viewMat = lookAt(cam.pos, cam.look, cam.up);
	glUniformMatrix4fv(viewMatLoc, 1, 0, value_ptr(viewMat));

	GLuint camPosLoc = glGetUniformLocation(program.programID, "cameraPos");
	glUniform3fv(camPosLoc, 1, value_ptr(cam.pos));

	// fov 설정
	GLuint projMatLoc = glGetUniformLocation(program.programID, "projMat");
	mat4 projMat = perspective(radians(cam.fov), 1024.f / 768.f, 0.1f, 100.0f);
	glUniformMatrix4fv(projMatLoc, 1, 0, value_ptr(projMat));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
	glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}

