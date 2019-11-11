//
//  main.cpp
//  ComputerGraphics_2
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
// 추가한 함수들
#include <glm/gtx/transform.hpp>
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

// 회전을 위해 추가
GLfloat rotateAngle = 0;

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

void initialize() {
	testProgram.loadShaders("shader.vert", "shader.frag");

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	//	float data[] = {-1.0, -1.0, 0.0,  1.0, -1.0, 0.0,  0.0, 1.0, 0.0};
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(-1.0, -1.0, 0.0));
	vertices.push_back(glm::vec3(1.0, -1.0, 0.0));
	vertices.push_back(glm::vec3(0.0, 1.0, 0.0));

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	std::vector<glm::u16vec3> indices;
	indices.push_back(glm::u16vec3(0, 1, 2));
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

	// 셰이더에 정의된 uniform의 위치를 얻는다
	GLuint loc = glGetUniformLocation(testProgram.programID, "modelMat");
	glm::mat4 rotatedMatrix = glm::rotate(rotateAngle, glm::vec3(0, 1, 0));
	// uniform의 값을 수정한다
	glUniformMatrix4fv(loc, 1, 0, glm::value_ptr(rotatedMatrix));

	rotateAngle += 0.001;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
	//	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);

	glfwSwapBuffers(window);
}

