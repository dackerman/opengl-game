/*
 * ShaderProgram.hpp
 *
 *  Created on: Apr 29, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef SHADERPROGRAM_HPP_
#define SHADERPROGRAM_HPP_

#include "shader.hpp"

#include <glm/glm.hpp>

class ShaderProgram {
public:
	ShaderProgram(Shader * vertexShader, Shader * fragmentShader);
	void link();
	void activate();
	void setMatrixValue(const char * variableName, glm::mat4 matrix);
	GLint id();

private:
	Shader * vertexShader;
	Shader * fragmentShader;

	GLint programId;
};


#endif /* SHADERPROGRAM_HPP_ */
