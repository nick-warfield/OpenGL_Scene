#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <tuple>

// world space input --> screen space
// that means it has to have a view matrix and projection matrix

struct Camera {
	// constructs projection matrix
	float fov;
	float aspect_ratio;
	float near_plane;
	float far_plane;

	glm::vec3 position;
	glm::vec3 rotation;			// x = pitch, y = yaw, z = roll
};

Camera make_default_camera();

glm::mat4 get_view(Camera camera, glm::vec3 world_up);
glm::mat4 get_projection(Camera camera);
std::tuple<glm::vec3, glm::vec3, glm::vec3> get_vectors(Camera Camera, glm::vec3 world_up);

void move(Camera &camera, glm::vec3 direction, float speed, float delta);
void move_local(Camera &camera, glm::vec3 direction, float speed, float delta);
void rotate(Camera &camera, float xoffset, float yoffset, float sensitivity);
void zoom(Camera &camera, float yoffset);

#endif
