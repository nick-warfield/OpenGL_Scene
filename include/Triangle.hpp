#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

class Triangle {
public:
	Shader shader;
	unsigned int VBO, VAO, EBO;
	unsigned int indices[3] = { 0, 1, 2 };
	float vertices[];

	Triangle(float verts[], unsigned int size, Shader shader_program) :
	shader(shader_program) {
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void draw(void) {
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
};
