//
//  main.cpp
//  ComputerGraphics_4
//
//  Created by Hyun Joon Shin on 13/03/2018.
//  Copyright 짤 2018 Hyun Joon Shin. All rights reserved.
//

#include "toys.h"
#include <glfw/glfw3.h>
#ifdef WIN32
#include <gl/glew.h>
#endif
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef WIN32
#pragma comment (lib, "glew32s")
#pragma comment (lib, "glew32")
#pragma comment (lib, "opengl32")
#pragma comment (lib, "glfw3")
#endif

void render(GLFWwindow* window);
void initialize();

GLuint triangleVBO;
GLuint vertexArrayID;
GLuint indexVBOID;

Program testProgram;

int main(int argc, const char * argv[]) {

	if (!glfwInit())        exit(EXIT_FAILURE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
	glfwMakeContextCurrent(window);

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

const double PI = 3.1415926535;
// 모서리 갯수. 이 값이 크면 원이 된다
const int EDGE_COUNT = 24;

void initialize() {
	testProgram.loadShaders("shader.vert", "shader.frag");

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// vertex 정보 생성
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(0, 0, 0));
	for (int i = 1; i <= EDGE_COUNT; i++) {
		double theta = 2 * PI / EDGE_COUNT * i;
		vertices.push_back(glm::vec3(cos(theta), sin(theta), 0));
	}

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// index 정보 생성
	std::vector<glm::u16vec3> indices;
	for (int i = 1; i <= EDGE_COUNT; i++) 
		indices.push_back(glm::u16vec3(0, i, i+1));
	indices.push_back(glm::u16vec3(0, EDGE_COUNT, 1));

	glGenBuffers(1, &indexVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::u16vec3)*indices.size(), indices.data(), GL_STATIC_DRAW);
}

void render(GLFWwindow* window) {
	int width, height;
	
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0, 0, .5, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(testProgram.programID);
	glBindVertexArray(vertexArrayID);

	glDrawElements(GL_TRIANGLES, (EDGE_COUNT+1) * 3, GL_UNSIGNED_SHORT, 0);

	glfwSwapBuffers(window);
}

