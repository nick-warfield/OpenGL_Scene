#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	uint ID;

	Shader(const char* vertex_path, const char* fragment_path) {
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

		int success;
		char infoLog[512];

		uint v_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v_shader, 1, &v_shader_code, NULL);
		glCompileShader(v_shader);
		glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(v_shader, 512, NULL, infoLog);
			std::cout << "ERROR: vertex shader compilation failed\n" << infoLog << std::endl;
		}

		uint f_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f_shader, 1, &f_shader_code, NULL);
		glCompileShader(f_shader);
		glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(f_shader, 512, NULL, infoLog);
			std::cout << "ERROR: fragment shader compilation failed\n" << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, v_shader);
		glAttachShader(ID, f_shader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR: shader program linking failed\n" << infoLog << std::endl;
		}

		glDeleteShader(v_shader);
		glDeleteShader(f_shader);
	}

	void use(void) { glUseProgram(ID); }
	void set_bool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void set_int(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void set_float(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
};

#endif
