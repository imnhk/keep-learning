#include "graphics.h"
#include "main.h"
#include "obj.h"

#include <vector>

#define PATH "../../Assignment #1/Graphics/"

using namespace glm;
using namespace std;

vector<glm::vec3> vertices;
vector<glm::vec2> uvs;
vector<glm::vec3> normals;

int vertexShader = glCreateShader(GL_VERTEX_SHADER);


GLuint vertexBuffer; 
GLuint uvBuffer;
GLuint normalBuffer;

Camera cam;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main(int argc, const char * argv[]) {

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Namhyeon Kim 201521053", NULL, NULL);
	if (window == NULL) {
		printf("Failed to open GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//callback 사용 설정
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, buttonCallback);
	glfwSetScrollCallback(window, scrollCallback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	initialize();

	// Window 열려 있을 때
	while (!glfwWindowShouldClose(window)) {
		// 키보드 입력
		processInput(window);

		glClearColor(.2f, .1f, .1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

void initialize() {
	
	//program.loadShaders("shader.vert", "shader.frag");
	
	loadOBJ("spot_triangulated.obj", vertices, uvs, normals);

	// Load into Vertex buffer object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);


	/*

	// Vertex Array 생성, 활성화
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// buffer의 데이터로 수정할 vertex attribute의 index 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(1);

	// Index buffer 생성
	glGenBuffers(1, &indexVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nTriangles[0] * sizeof(u32vec3), triangles[0], GL_STATIC_DRAW);
	*/
}


void render(GLFWwindow* window) {
	/*
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
		vec4(0, 0, cam.DIST, 0);

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
	//glDrawElements(GL_TRIANGLES, nTriangles[0] * 3, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	*/
}

void cursorCallback(GLFWwindow* win, double x, double y) {
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1)) {
		cam.yaw -= (x - cam.lastX) / 200;
		cam.pitch = clamp<float>(cam.pitch - ((y - cam.lastY) / 200), PI / -2 + 0.1f, PI / 2 - 0.1f);
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
	cam.fov = clamp<double>(cam.fov -= yOffset * 2, cam.FOV_MIN, cam.FOV_MAX);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}