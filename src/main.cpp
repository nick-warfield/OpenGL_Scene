#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Shape.hpp"
#include "Camera.hpp"

#include <iostream>
#include <cmath>

Cam camera = make_default_camera();
bool first_mouse = true;
float lastX = 400.0f, lastY = 300.0f;

void processInput(GLFWwindow*);
void framebuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double _xoffset, double yoffset);

Shape make_cube(void);

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

	Shape rect = make_cube();

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

		// this should all be handled by camera, probably
		glm::mat4 view = get_view(camera, glm::vec3(0, 1, 0));
		int viewLoc = glGetUniformLocation(rect.shader, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = get_projection(camera);
		int projectionLoc = glGetUniformLocation(rect.shader, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//float greenValue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
		//rect.shader.set_float("ourColor", greenValue);

		glm::mat4 trans = glm::mat4(1.0);
		trans = glm::translate(trans, glm::vec3(1, -1, 1));
		trans = glm::scale(trans, glm::vec3(1, 0.25, 1));
		draw_shape(rect, trans);

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

Shape make_cube(void) {
	std::vector<float> verts = {
		// positions			// colors			// texture coords
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f
	};

	std::vector<int> indices = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	std::vector<Attribute> attributes;
	attributes.push_back(make_attribute(3, GL_FLOAT, false, sizeof(float)));
	attributes.push_back(make_attribute(3, GL_FLOAT, false, sizeof(float)));
	attributes.push_back(make_attribute(2, GL_FLOAT, false, sizeof(float)));

	std::vector<uint> textures;
	textures.push_back(make_texture("resources/container.jpg"));
	textures.push_back(make_texture("resources/awesomeface.png"));

	uint shader = make_shader("resources/vertex_shader.glsl", "resources/fragment_shader.glsl");

	return make_shape(verts, indices, attributes, textures, shader);
}

