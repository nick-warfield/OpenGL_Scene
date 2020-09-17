#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tuple>

// easy local space manipulation
// easy world space conversion
// basically, creates a model matix
struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;			// x = pitch, y = yaw, z = roll
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

std::tuple<glm::vec3, glm::vec3, glm::vec3> get_vectors(const Transform &transform, const glm::vec3 &world_up) {
	glm::vec3 front;
	front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
	front.y = sin(glm::radians(transform.rotation.x));
	front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));

	front = glm::normalize(front);
	auto right = glm::normalize(glm::cross(front, world_up));
	auto up = glm::normalize(glm::cross(right, front));
	return std::make_tuple(right, up, front);
}

void move(Transform &cam, glm::vec3 direction, float speed, float delta) {
	cam.position += glm::normalize(direction) * speed * delta;
}

void move_local(Transform &cam, glm::vec3 direction, float speed, float delta) {
	glm::vec3 right, up, front;
	std::tie(right, up, front) = get_vectors(cam, glm::vec3(0, 1, 0));
	move(cam, glm::mat3(right, up, front) * direction, speed, delta);
}

void rotate(Transform &cam, float xoffset, float yoffset, float sensitivity) {
	auto yaw = cam.rotation.y + xoffset * sensitivity;
	auto pitch = cam.rotation.x + yoffset * sensitivity;
	pitch = fmin(pitch, 89.0f);
	pitch = fmax(pitch, -89.0f);
	cam.rotation.x = pitch;
	cam.rotation.y = yaw;
}

#endif
