// Fragment Shader
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPosition;
in vec3 Normal;

uniform sampler2D texture0;

uniform Material material;
uniform Light light;

uniform vec3 view_position;

void main() {
	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(light.position - FragPosition);
	vec3 view_dir = normalize(view_position - FragPosition);
	vec3 reflect_dir = reflect(-light_dir, norm);

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// diffuse
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

	// specular
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	vec3 color = ambient + diffuse + specular;
	FragColor = vec4(color, 1.0);
}

