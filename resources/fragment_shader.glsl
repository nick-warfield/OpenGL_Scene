// Fragment Shader
#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 light_color;
uniform sampler2D texture0;

void main() {
	FragColor = vec4(light_color, 1.0) * texture(texture0, TexCoord);
}

