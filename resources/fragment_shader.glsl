// Fragment Shader
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPosition;
in vec3 Normal;

uniform sampler2D texture0;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main() {
	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(light_position - FragPosition);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	// specular
	float specular_strength = 0.5f;
	vec3 view_dir = normalize(view_position - FragPosition);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * light_color;

	vec3 color = ambient + diffuse + specular;
	FragColor = vec4(color * vec3(1.0, 0.5, 0.31), 1.0);
	//FragColor = vec4(color, 1.0) * texture(texture0, TexCoord);
}

