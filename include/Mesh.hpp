#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.hpp"

#include <vector>
#include <string>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coordinate;
};

struct Mesh {
	unsigned int vertex_buffer_object;
	unsigned int vertex_array_object;
	unsigned int element_buffer_object;

	std::vector<Vertex> vertex;
	std::vector<unsigned int> index;
	std::vector<Texture> texture;
};

void draw_mesh(const Mesh& mesh, unsigned int shader) {
	// activate, bind, and set textures
	for (int i = 0; i < mesh.texture.size(); ++i) {
		bind_texture(mesh.texture[i].id, i);
		glUniform1i(glGetUniformLocation(shader, mesh.texture[i].name.c_str()), i);
	}

	// draw mesh
	glBindVertexArray(mesh.vertex_array_object);
	glDrawElements(GL_TRIANGLES, mesh.index.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh make_mesh(
		std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<Texture> textures)
{
	Mesh m;
	m.vertex = vertices;
	m.index = indices;
	m.texture = textures;

 	// generate buffers
 	glGenBuffers(1, &m.vertex_buffer_object);
 	glGenVertexArrays(1, &m.vertex_array_object);
 	glGenBuffers(1, &m.element_buffer_object);

	// bind buffers
 	glBindVertexArray(m.vertex_array_object);
 	glBindBuffer(GL_ARRAY_BUFFER, m.vertex_buffer_object);
 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.element_buffer_object);
 
	// load data
 	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// handle attribute settings
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));
	glEnableVertexAttribArray(2);

	// Unbind Vertex Array Buffer
	glBindVertexArray(0);
	return m;
}

#endif
