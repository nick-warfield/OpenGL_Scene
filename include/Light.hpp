#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Transform.hpp"

#include <string>

struct Light {
	Transform transform;
	const std::string name;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct DirectionalLight : Light {
	glm::vec3 direction;
};

struct PointLight : Light {
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight : Light {
	glm::vec3 direction;
	float inner_cutoff;
	float outer_cutoff;

	float constant;
	float linear;
	float quadratic;
};

void set_light_properties(Light light, unsigned int shader) {
	glUniform3f(glGetUniformLocation(shader, (light.name + ".position").c_str()), light.transform.position.x, light.transform.position.y, light.transform.position.z);
	glUniform3f(glGetUniformLocation(shader, (light.name + ".ambient").c_str()), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader, (light.name + ".diffuse").c_str()), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader, (light.name + ".specular").c_str()), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader, (light.name + ".constant").c_str()), 1.0f);
	glUniform1f(glGetUniformLocation(shader, (light.name + ".linear").c_str()), 0.09f);
	glUniform1f(glGetUniformLocation(shader, (light.name + ".quadratic").c_str()), 0.032f);
}

Light make_light() {
	Light l;
	return l;
}

#endif
