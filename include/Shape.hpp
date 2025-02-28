// this should really be a model struct/class

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <tuple>

struct Attribute {
	unsigned int count;
	unsigned int type;
	bool should_normalize;
	size_t size;
};

// this is really a mesh with some extras
struct Shape {
	unsigned int vertex_buffer_object;
	unsigned int vertex_array_object;
	unsigned int element_buffer_object;
	unsigned int number_of_points;

	unsigned int shader;
	std::vector<unsigned int> texture;
};

Attribute make_attribute(
		unsigned int count,
		unsigned int type,
		bool should_normalize,
		size_t size);

Shape make_shape(
		std::vector<float> verts,
		std::vector<int> indices,
		std::vector<Attribute> attributes,
		std::vector<std::tuple<unsigned int, std::string>> textures,
		unsigned int shader);
void draw_shape(Shape shape, glm::mat4 transform);

#endif
