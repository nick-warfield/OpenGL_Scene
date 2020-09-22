#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Model.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <iostream>
#include <cmath>

Camera camera = make_default_camera();
bool first_mouse = true;
float lastX = 400.0f, lastY = 300.0f;

void processInput(GLFWwindow*);
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double _xoffset, double yoffset);

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Load Glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);

	Model model = load_model(std::string("resources/backpack/backpack.obj"));
	std::cout << std::endl;
	//auto tex = make_texture(std::string("blag"), std::string("resources/backpack/diffuse.jpg"));

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(0);

		// swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	float speed = 10, delta = 0.01f;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move_local(camera, glm::vec3(0.0, 0.0, 1.0), speed, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		move_local(camera, glm::vec3(0.0, 0.0, -1.0), speed, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		move_local(camera, glm::vec3(-1.0, 0.0, 0.0), speed, delta);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		move_local(camera, glm::vec3(1.0, 0.0, 0.0), speed, delta);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (first_mouse) {
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	rotate(camera, xoffset, yoffset, 0.1f);
}

void scroll_callback(GLFWwindow* window, double _xoffset, double yoffset) {
	zoom(camera, yoffset);
}

void framebuffer_size_callback(
		GLFWwindow* window,
		int width,
		int height
	) {
	glViewport(0, 0, width, height);
}

