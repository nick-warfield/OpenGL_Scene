// Vertex Shader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPosition;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	FragPosition = vec3(model * vec4(aPos, 1.0));
	// avoid inversing matricies in shaders!
	Normal = mat3(transpose(inverse(model))) * aNormal;
}

