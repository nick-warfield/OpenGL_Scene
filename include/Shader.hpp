#ifndef SHADER_HPP
#define SHADER_HPP

unsigned int make_shader(const char* vertex_path, const char* fragment_path);
unsigned int compile_shader(unsigned int SHADER_TYPE, const char * shader_code);
unsigned int link_shader_program(unsigned int vertex_shader, unsigned int fragment_shader);

#endif
