#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

struct Texture {
	unsigned int id;
	std::string name;
	std::string path;
};

Texture make_texture(std::string name, std::string texture_path);
unsigned int load_texture(const char * texture_path);
void bind_texture(unsigned int texture, unsigned int texture_unit);

#endif
