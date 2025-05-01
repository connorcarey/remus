#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

// Refer to the header file for documentation on functions.
struct Shader {

	unsigned int ID; // Program ID.

	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCESSFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		GLuint* shaders = new GLuint[2]; // {vertex, fragment}
		compileShader(shaders[0], vShaderCode, GL_VERTEX_SHADER);
		compileShader(shaders[1], fShaderCode, GL_FRAGMENT_SHADER);
		createProgram(shaders, 2);
	}

	void use() {
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:

	void compileShader(GLuint &shader, const GLchar* shaderCode, GLenum type) {
		shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED::" << type << "\n" << infoLog << std::endl;
		}
	}

	void createProgram(GLuint shaders[], size_t size) {
		ID = glCreateProgram();
		for (size_t i = 0; i < size; ++i) {
			glAttachShader(ID, shaders[i]);
		}

		glLinkProgram(ID);

		for (size_t i = 0; i < size; ++i) {
			glDeleteShader(shaders[i]);
		}

		int success;
		char infoLog[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
};
