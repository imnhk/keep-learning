#include "main.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = 0;
double lastY = 0;

enum RenderMode{NdotV, Silhouettes, BlurredSilouettes};
RenderMode renderMode = BlurredSilouettes;

int main(int argc, char* argv[])
{
	// GLFW 설정
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLWF wondow 생성
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, buttonCallback);

	// GLAD 불러오기
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL 설정
	glEnable(GL_DEPTH_TEST);

	// Shader를 불러와서 compile
	Shader shader("src/vertex.shader", "src/fragment.shader");

	// 모델 불러오기
	// console argument 로 파일명을 입력받을 수 있다
	string modelName;
	if (argc > 1)
		modelName = argv[0];
	else
		modelName = "spot_triangulated.obj";
	cout << "Loading model " << modelName << "..." << endl;
	Model ourModel(modelName);
	cout << "Loaded model " << modelName << endl;

	


	// fullscreen quad buffer
	static const GLfloat quadVertexData[] = {
		// position			// texCoord
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	// renderTexture quad를 위한 VAO
	unsigned int quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	unsigned int quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(2);



	// multi-pass render를 위한 gBuffer
	unsigned int gBuffer = 0;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	unsigned int renderedTex;
	glGenTextures(1, &renderedTex);
	glBindTexture(GL_TEXTURE_2D, renderedTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTex, 0);
	unsigned int drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// 입력
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader.ID);

		// projection mat 적용
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// 모델 render
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setMat4("model", model);
		shader.setVec3("cameraPos", camera.Position);

		if (renderMode == NdotV)
		{
			shader.setInt("pass", 0);
			glBindVertexArray(ourModel.meshes[0].VAO);
			glDrawElements(GL_TRIANGLES, ourModel.meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}
		else if (renderMode == Silhouettes) {
			shader.setInt("pass", 1);
			//ourModel.Draw(shader);
			glBindVertexArray(ourModel.meshes[0].VAO);
			glDrawElements(GL_TRIANGLES, ourModel.meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

		}
		else if (renderMode == BlurredSilouettes)
		{
			// renderedTex에 render
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glUniform1i(glGetUniformLocation(shader.ID, "pass"), 1);

			glBindVertexArray(ourModel.meshes[0].VAO);
			glDrawElements(GL_TRIANGLES, ourModel.meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// 다시 원래 framebuffer로 복구
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glUniform1i(glGetUniformLocation(shader.ID, "pass"), 2);
			
			// quad 그리기
			glBindTexture(GL_TEXTURE_2D, renderedTex);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);


		
		}
		else
			return -1;


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		renderMode = NdotV;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		renderMode = Silhouettes;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		renderMode = BlurredSilouettes;
	}
}

// window 크기 변경 대응
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 마우스 스크롤
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// 드래그 회전
void cursorCallback(GLFWwindow* win, double x, double y) {
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1)) {
		double dx = x - lastX;
		double dy = y - lastY;
		lastX = x;
		lastY = y;

		camera.Yaw -= dx;
		camera.Pitch -= dy;
		camera.Pitch = glm::clamp<float>(camera.Pitch, -89.0, 89.0);;

		camera.Position =
			glm::rotate(camera.Yaw * 0.017f, glm::vec3(0, 1, 0)) *
			glm::rotate(camera.Pitch * 0.017f, glm::vec3(1, 0, 0)) *
			glm::vec4(0, 0, 3, 0);
	}
}

// 마우스 클릭 위치
void buttonCallback(GLFWwindow* win, int btn, int action, int mods) {
	if (btn == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		glfwGetCursorPos(win, &lastX, &lastY);
}