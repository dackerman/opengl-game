#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <string>
#include <GL/glew.h>


class Shader {
public:
	Shader(GLenum shaderType, std::string filename);
	
	GLuint id();
	std::string name();
	void compile();

private:
	GLenum type;
	std::string filename;
	GLuint compiledId;

	const char * getShader(const char * src);
};

#endif
