// Fragment Shader
#version 330 core
out vec4 FragColor;
in vec3 vertexColor;

uniform float ourColor;

void main() {
	FragColor = vec4(vertexColor.x, ourColor, vertexColor.z, 1.0f);
}

