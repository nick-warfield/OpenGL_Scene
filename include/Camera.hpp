#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// world space input --> screen space
// that means it has to have a view matrix and projection matrix

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
