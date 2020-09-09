#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Shader.hpp"
#include "Rectangle.hpp"

#include <glm/trigonometric.hpp>
#include <iostream>
#include <cmath>

void processInput(GLFWwindow*);
void framebuffer_size_callback(GLFWwindow*, int, int);

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

	Shader shader("resources/vertex_shader.glsl", "resources/fragment_shader.glsl");

	float verts[] = {
		// positions			// colors			// texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,		// top left
	};

	Rectangle rect(verts, sizeof(verts), shader, "resources/container.jpg");

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float greenValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
		rect.shader.set_float("ourColor", greenValue);

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

		unsigned int transformLoc = glGetUniformLocation(rect.shader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		rect.draw();

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.75f, 0.65f, 0.0f));
		trans = glm::rotate(trans, glm::radians(167.0f), glm::vec3(0.0, 0.0, 1.0));
		greenValue += 0.5f;
		trans = glm::scale(trans, glm::vec3(greenValue, greenValue, greenValue));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		rect.draw();

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
