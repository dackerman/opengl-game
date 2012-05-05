/*
 * ShaderProgram.hpp
 *
 *  Created on: Apr 29, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef SHADERPROGRAM_HPP_
#define SHADERPROGRAM_HPP_

#include "Shader.hpp"

#include <glm/glm.hpp>

class ShaderProgram {
public:
	ShaderProgram(Shader * vertexShader, Shader * fragmentShader);
	void link();
	void activate();
	void setMatrix(const char * variableName, glm::mat4 matrix);
	void setVector(const char * variableName, glm::vec4 vec);
	void setVector(const char * variableName, glm::vec3 vec);
	void setFloat(const char * variableName, float fl);
	GLint id();

private:
	Shader * vertexShader;
	Shader * fragmentShader;

	GLint programId;

	int loc(const char * name);
};


#endif /* SHADERPROGRAM_HPP_ */
