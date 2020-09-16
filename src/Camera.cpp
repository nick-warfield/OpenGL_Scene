#include "Camera.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <tuple>

Camera make_default_camera() {
	Camera c;
	c.fov = 45;
	c.aspect_ratio = 800.0 / 600.0;
	c.near_plane = 0.1f;
	c.far_plane = 1000.0f;
	c.position = glm::vec3(0.0, 0.0, 5.0);
	c.rotation = glm::vec3(0.0, -90.0, 0.0);
	return c;
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> get_vectors(Camera cam, glm::vec3 world_up) {
	glm::vec3 front;
	front.x = cos(glm::radians(cam.rotation.y)) * cos(glm::radians(cam.rotation.x));
	front.y = sin(glm::radians(cam.rotation.x));
	front.z = sin(glm::radians(cam.rotation.y)) * cos(glm::radians(cam.rotation.x));

	front = glm::normalize(front);
	auto right = glm::normalize(glm::cross(front, world_up));
	auto up = glm::normalize(glm::cross(right, front));
	return std::make_tuple(right, up, front);
}

glm::mat4 get_view(Camera cam, glm::vec3 world_up) {
	glm::vec3 _, up, front;
	std::tie(_, up, front) = get_vectors(cam, world_up);
	return glm::lookAt(cam.position, cam.position + front, up);
}

glm::mat4 get_projection(Camera cam) {
	return glm::perspective(glm::radians(cam.fov), cam.aspect_ratio, cam.near_plane, cam.far_plane);
}

void move(Camera &cam, glm::vec3 direction, float speed, float delta) {
	cam.position += glm::normalize(direction) * speed * delta;
}

void move_local(Camera &cam, glm::vec3 direction, float speed, float delta) {
	glm::vec3 right, up, front;
	std::tie(right, up, front) = get_vectors(cam, glm::vec3(0, 1, 0));
	move(cam, glm::mat3(right, up, front) * direction, speed, delta);
}

void rotate(Camera &cam, float xoffset, float yoffset, float sensitivity) {
	auto yaw = cam.rotation.y + xoffset * sensitivity;
	auto pitch = cam.rotation.x + yoffset * sensitivity;
	pitch = fmin(pitch, 89.0f);
	pitch = fmax(pitch, -89.0f);
	cam.rotation.x = pitch;
	cam.rotation.y = yaw;
}

void zoom(Camera &cam, float yoffset) {
	cam.fov -= (float)yoffset;
	if (cam.fov < 1.0f) { cam.fov = 1.0f; }
	if (cam.fov > 45.0f) { cam.fov = 45.0f; }
}
