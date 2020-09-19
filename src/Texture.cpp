#include "Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Texture make_texture(std::string name, std::string texture_path) {
	Texture t;
	t.id = load_texture(texture_path.c_str());
	t.name = name;
	t.path = texture_path;
	return t;
}

unsigned int load_texture(const char * texture_path) {
	unsigned int texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		switch (nrChannels) {
			case 1: format = GL_RED; break;
			case 3: format = GL_RGB; break;
			case 4: format = GL_RGBA; break;
		}

		// load texture data
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set texture paramters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "Failed to load texture from: " << texture_path << std::endl;
	}

	stbi_image_free(data);
	return texture;
}

void bind_texture(uint texture, uint texture_unit) {
	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

