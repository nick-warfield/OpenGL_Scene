#include "Shader.hpp"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

unsigned int make_shader(const char* vertex_path, const char* fragment_path) {
	std::string v_code, f_code;
	std::ifstream v_shader_file ,f_shader_file;
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);

		std::stringstream v_shader_stream, f_shader_stream;
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		v_shader_file.close();
		f_shader_file.close();

		v_code = v_shader_stream.str();
		f_code = f_shader_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error: Shader file not succesfully read" << std::endl;
	}
	const char* v_shader_code = v_code.c_str();
	const char* f_shader_code = f_code.c_str();

	uint v_shader = compile_shader(GL_VERTEX_SHADER, v_shader_code);
	uint f_shader = compile_shader(GL_FRAGMENT_SHADER, f_shader_code);
	uint shader_program = link_shader_program(v_shader, f_shader);

	glDeleteShader(v_shader);
	glDeleteShader(f_shader);

	return shader_program;
}

uint compile_shader(uint SHADER_TYPE, const char * shader_code) {
	int success;
	char infoLog[512];
	uint shader = glCreateShader(SHADER_TYPE);
	glShaderSource(shader, 1, &shader_code, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		const char * type = SHADER_TYPE == GL_FRAGMENT_SHADER ? "fragment" : "vertex";
		std::cout << "ERROR: " << type << " shader compilation failed\n" << infoLog << std::endl;
	}
	return shader;
}

uint link_shader_program(uint vertex_shader, uint fragment_shader) {
	int success;
	char infoLog[512];
	unsigned int shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "ERROR: shader program linking failed\n" << infoLog << std::endl;
	}
	return shader_program;
}

