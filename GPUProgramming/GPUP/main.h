#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// ���콺 �巡�� �Է��� ���ؼ�
void cursorCallback(GLFWwindow* win, double x, double y);
void buttonCallback(GLFWwindow* win, int btn, int action, int mods);

// ���
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

