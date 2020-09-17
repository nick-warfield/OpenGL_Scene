// Fragment Shader
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPosition;
in vec3 Normal;

uniform sampler2D texture0;

uniform Material material;

#define NUMBER_OF_POINT_LIGHTS 1
uniform PointLight point_light[NUMBER_OF_POINT_LIGHTS];
uniform DirectionalLight directional_light;

uniform vec3 view_position;

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction);

void main() {
	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(view_position - FragPosition);

	vec3 color = calc_directional_light(directional_light, norm, view_dir);
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i) {
		color += calc_point_light(point_light[i], norm, FragPosition, view_dir);
	}
	// spot lights

	FragColor = vec4(color, 1.0);
}

vec3 calc_directional_light(DirectionalLight light, vec3 normal, vec3 view_direction) {
	vec3 light_dir = normalize(-light.direction);
	vec3 reflect_dir = reflect(-light_dir, normal);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// diffuse
	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	// specular
	float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_position, vec3 view_direction) {
	vec3 light_dir = normalize(light.position - frag_position);
	vec3 reflect_dir = reflect(-light_dir, normal);

	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// diffuse
	float diff = max(dot(normal, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	// specular
	float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	return ((ambient + diffuse + specular) * attenuation);
}

