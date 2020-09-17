#include "Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

unsigned int make_texture(const char * texture_path) {
	unsigned int texture;
	glGenTextures(1, &texture);
	bind_texture(texture, 0);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void bind_texture(uint texture, uint texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

