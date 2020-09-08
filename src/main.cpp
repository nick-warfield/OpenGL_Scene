#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n layout (location = 0) in vec3 aPos; void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }\0";
const char *l_fragmentShaderSource = "#version 330 core\n out vec4 FragColor; void main() { FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); }\0";
const char *r_fragmentShaderSource = "#version 330 core\n out vec4 FragColor; void main() { FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); }\0";


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Scene", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int success;
	char infoLog[512];

	// create, compile, and link shaders
	uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: vertex shader compilation failed\n" << infoLog << std::endl;
	}

	uint l_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(l_fragmentShader, 1, &l_fragmentShaderSource, NULL);
	glCompileShader(l_fragmentShader);
	glGetShaderiv(l_fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(l_fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: fragment shader compilation failed\n" << infoLog << std::endl;
	}

	uint r_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(r_fragmentShader, 1, &r_fragmentShaderSource, NULL);
	glCompileShader(r_fragmentShader);
	glGetShaderiv(r_fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(r_fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: fragment shader compilation failed\n" << infoLog << std::endl;
	}

	uint l_shaderProgram;
	l_shaderProgram = glCreateProgram();
	glAttachShader(l_shaderProgram, vertexShader);
	glAttachShader(l_shaderProgram, l_fragmentShader);
	glLinkProgram(l_shaderProgram);
	glGetProgramiv(l_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(l_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: shader program linking failed\n" << infoLog << std::endl;
	}

	uint r_shaderProgram;
	r_shaderProgram = glCreateProgram();
	glAttachShader(r_shaderProgram, vertexShader);
	glAttachShader(r_shaderProgram, r_fragmentShader);
	glLinkProgram(r_shaderProgram);
	glGetProgramiv(r_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(r_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: shader program linking failed\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(l_fragmentShader);
	glDeleteShader(r_fragmentShader);

	float l_verts[] = {
		-0.5f, -0.5f, 0.0f,
		-1.0f,  0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};
	float r_verts[] = {
		 0.5f,  0.5f, 0.0f,
		 1.0f, -0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
	};
	uint indices[] = {
		0, 1, 2,
	};

	uint l_VBO, l_VAO, r_VBO, r_VAO, EBO;
	glGenBuffers(1, &l_VBO);
	glGenBuffers(1, &r_VBO);
	glGenVertexArrays(1, &l_VAO);
	glGenVertexArrays(1, &r_VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(l_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, l_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(l_verts), l_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(r_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, r_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(r_verts), r_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(l_shaderProgram);
		glBindVertexArray(l_VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glUseProgram(r_shaderProgram);
		glBindVertexArray(r_VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(
		GLFWwindow* window,
		int width,
		int height
	) {
	glViewport(0, 0, width, height);
}
