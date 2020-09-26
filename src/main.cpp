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

	std::vector<Vertex> vertices = {
		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec2(0.0f, 0.0f)),

		make_vertex(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec2(0.0f, 0.0f)),

		make_vertex(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec2(1.0f, 0.0f)),

		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f, -0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec2(1.0f, 0.0f)),

		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f,  0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f, -0.5f, -0.5f),	glm::vec3(0.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 1.0f)),

		make_vertex(glm::vec3(-0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f), 	glm::vec2(0.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f, -0.5f),	glm::vec3(0.0f, 1.0f, 0.0f), 	glm::vec2(1.0f, 1.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 1.0f, 0.0f), 	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3( 0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 1.0f, 0.0f), 	glm::vec2(1.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f,  0.5f),	glm::vec3(0.0f, 1.0f, 0.0f), 	glm::vec2(0.0f, 0.0f)),
		make_vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 1.0f))
	};

	std::vector<uint> indices = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	};

	std::vector<Texture> textures = {
		make_texture("diffuse", "resources/container2.png"),
		make_texture("specular", "resources/container2_specular.png")
	};

	Mesh mesh = make_mesh(vertices, indices, textures);
	uint shader = make_shader("resources/vertex_shader.glsl", "resources/fragment_shader.glsl");
	glm::mat4 trans = glm::mat4(1.0f);

	// set up direction light
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, "directional_light.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(shader, "directional_light.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader, "directional_light.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "directional_light.specular"), 1.0f, 1.0f, 1.0f);

	// wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		glUseProgram(shader);
		glUniformMatrix4fv(
				glGetUniformLocation(shader, "view"),
				1,
				GL_FALSE,
				glm::value_ptr(get_view(camera, glm::vec3(0, 1, 0))));
		glUniformMatrix4fv(
				glGetUniformLocation(shader, "projection"),
				1,
				GL_FALSE,
				glm::value_ptr(get_projection(camera)));

		glUniform3f(
				glGetUniformLocation(shader, "view_position"),
				camera.position.x,
				camera.position.y,
				camera.position.z);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw_model(model, shader, trans);
		//glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(trans));
		//draw_mesh(mesh, shader);

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

