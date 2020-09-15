#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// easy local space manipulation
// easy world space conversion
// basically, creates a model matix
struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

glm::mat4 get_matrix(Transform t) {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, t.position);
	trans = glm::rotate(trans, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));
	trans = glm::scale(trans, t.scale);
	return trans;
}

#endif
