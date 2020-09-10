#ifndef TEXTURE_HPP
#define TEXTURE_HPP

unsigned int make_texture(const char * texture_path);
void bind_texture(unsigned int texture, unsigned int texture_unit);

#endif
