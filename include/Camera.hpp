#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>
#include <vector>
#include <tuple>

// world space input --> screen space
// that means it has to have a view matrix and projection matrix

struct Cam {
	// constructs projection matrix
	float fov;
	float aspect_ratio;
	float near_plane;
	float far_plane;

	glm::vec3 position;
	glm::vec3 rotation;			// x = pitch, y = yaw, z = roll
};

Cam make_default_camera() {
	Cam c;
	c.fov = 45;
	c.aspect_ratio = 800.0 / 600.0;
	c.near_plane = 0.1f;
	c.far_plane = 1000.0f;
	c.position = glm::vec3(0.0, 0.0, 5.0);
	c.rotation = glm::vec3(0.0, -90.0, 0.0);
	return c;
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> get_vectors(Cam cam, glm::vec3 world_up) {
	glm::vec3 front;
	front.x = cos(glm::radians(cam.rotation.y)) * cos(glm::radians(cam.rotation.x));
	front.y = sin(glm::radians(cam.rotation.x));
	front.z = sin(glm::radians(cam.rotation.y)) * cos(glm::radians(cam.rotation.x));

	front = glm::normalize(front);
	auto right = glm::normalize(glm::cross(front, world_up));
	auto up = glm::normalize(glm::cross(right, front));
	return std::make_tuple(right, up, front);
}

glm::mat4 get_view(Cam cam, glm::vec3 world_up) {
	glm::vec3 _, up, front;
	std::tie(_, up, front) = get_vectors(cam, world_up);
	return glm::lookAt(cam.position, cam.position + front, up);
}

glm::mat4 get_projection(Cam cam) {
	return glm::perspective(glm::radians(cam.fov), cam.aspect_ratio, cam.near_plane, cam.far_plane);
}

void move(Cam &cam, glm::vec3 direction, float speed, float delta) {
	cam.position += glm::normalize(direction) * speed * delta;
}

void move_local(Cam &cam, glm::vec3 direction, float speed, float delta) {
	glm::vec3 right, up, front;
	std::tie(right, up, front) = get_vectors(cam, glm::vec3(0, 1, 0));
	move(cam, glm::mat3(right, up, front) * direction, speed, delta);
}

void rotate(Cam &cam, float xoffset, float yoffset, float sensitivity) {
	auto yaw = cam.rotation.y + xoffset * sensitivity;
	auto pitch = cam.rotation.x + yoffset * sensitivity;
	pitch = fmin(pitch, 89.0f);
	pitch = fmax(pitch, -89.0f);
	cam.rotation.x = pitch;
	cam.rotation.y = yaw;
}

void zoom(Cam &cam, float yoffset) {
	cam.fov -= (float)yoffset;
	if (cam.fov < 1.0f) { cam.fov = 1.0f; }
	if (cam.fov > 45.0f) { cam.fov = 45.0f; }
}

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 position, front, up, world_up, right;
	float yaw, pitch, speed, mouse_sensitivity, zoom;

	Camera(glm::vec3 Position) :
		position(Position),
		front(glm::vec3(0.0, 0.0, -1.0)),
		up(glm::vec3(0.0, 1.0, 0.0)),
		world_up(glm::vec3(0.0, 1.0, 0.0)),
		right(glm::vec3(1.0, 0.0, 0.0)),
		yaw(YAW),
		pitch(PITCH),
		speed(SPEED),
		mouse_sensitivity(SENSITIVITY),
		zoom(ZOOM)
	{ update_vectors(); }

	glm::mat4 look(void) {
		return glm::lookAt(position, position + front, up);
	}

	void move(CameraMovement direction, float deltaTime) {
		float velocity = speed * deltaTime;
		switch (direction) {
			case FORWARD: position += front * velocity; break;
			case BACKWARD: position -= front * velocity; break;
			case LEFT: position -= right * velocity; break;
			case RIGHT: position += right * velocity; break;
		}
	}

	void mouse_move(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= mouse_sensitivity;
		yoffset *= mouse_sensitivity;
		yaw += xoffset;
		pitch += yoffset;

		if (constrainPitch) {
			if (pitch > 89.0f) { pitch = 89.0f; }
			if (pitch < -89.0f) { pitch = -89.0f; }
		}
		
		update_vectors();
	}

	void mouse_scroll(float yoffset) {
		zoom -= (float)yoffset;
		if (zoom < 1.0f) { zoom = 1.0f; }
		if (zoom > 45.0f) { zoom = 45.0f; }
	}

private:
	void update_vectors(void) {
		glm::vec3 _front;
		_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		_front.y = sin(glm::radians(pitch));
		_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(_front);
		right = glm::normalize(glm::cross(front, world_up));
		up = glm::normalize(glm::cross(right, front));
	}
};

#endif
