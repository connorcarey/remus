#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

struct Shader {

	unsigned int ID; // Program ID.

	Shader(const char* vertexPath, const char* fragmentPath); // Constructor.

	void use(); // Use and activate the shader.

	// Utility functions for uniform variables.
	void setBool(const std::string& name, bool value) const; 
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:

	void compileShader(GLuint& shader, const GLchar* shaderCode, GLenum type);
	void createProgram(GLuint shaders[], size_t size);

};

#endif
