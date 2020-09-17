#include <Shape.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

Attribute make_attribute(
		unsigned int count,
		unsigned int type,
		bool should_normalize,
		size_t size) {
	Attribute a;
	a.count = count;
	a.type = type;
	a.should_normalize = should_normalize;
	a.size = size;
	return a;
}

void draw_shape(Shape shape, glm::mat4 transform) {
	//glUseProgram(shape.shader);

	for (int i = 0; i < shape.texture.size(); ++i) {
		bind_texture(shape.texture[i], i);
		std::string name = "texture" + std::to_string(i);
		glUniform1i(glGetUniformLocation(shape.shader, name.c_str()), i);
	}

	glUniformMatrix4fv(glGetUniformLocation(shape.shader, "model"), 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(shape.vertex_array_object);
	glDrawElements(GL_TRIANGLES, shape.number_of_points, GL_UNSIGNED_INT, 0);
}

Shape make_shape(
		std::vector<float> verts,
		std::vector<int> indices,
		std::vector<Attribute> attributes,
		std::vector<uint> textures,
		unsigned int shader) {
	Shape s;
	s.texture = textures;
	s.shader = shader;
	s.number_of_points = indices.size();

	// vertex stuff
	glGenBuffers(1, &s.vertex_buffer_object);
	glGenVertexArrays(1, &s.vertex_array_object);
	glGenBuffers(1, &s.element_buffer_object);

	glBindVertexArray(s.vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, s.vertex_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.element_buffer_object);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// attribute stuff
	size_t size = 0;
	for (auto a : attributes) { size += a.size * a.count; }

	int i = 0;
	size_t offset = 0;
	for (auto a : attributes) {
		glVertexAttribPointer(i, a.count, a.type, a.should_normalize, size, (void*)offset);
		glEnableVertexAttribArray(i);
		offset += a.size * a.count;
		++i;
	}
	return s;
}
