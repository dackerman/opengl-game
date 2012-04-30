#include <string>
#include <GL/glew.h>

class Shader {
public:
	Shader(GLenum shaderType, const char * source);
	
	void compile();

	GLuint id();

private:
	GLenum type;
	const char * filename;
	GLuint compiledId;
	const char * getShader(const char * src);
};
