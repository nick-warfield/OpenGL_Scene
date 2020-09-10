// Fragment Shader
#version 330 core

out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;

uniform float ourColor;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2)
		* vec4(VertexColor.x, ourColor, VertexColor.z, 1.0);
}

