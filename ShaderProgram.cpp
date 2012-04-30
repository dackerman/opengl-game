/*
 * ShaderProgram.cpp
 *
 *  Created on: Apr 29, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include "ShaderProgram.hpp"
#include "shader.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

ShaderProgram::ShaderProgram(Shader * vertexShader, Shader * fragmentShader) {
	this->vertexShader = vertexShader;
	this->fragmentShader = fragmentShader;
}

void ShaderProgram::link() {
	vertexShader->compile();
	fragmentShader->compile();

	GLint programObject = glCreateProgram();
	this->programId = programObject;

	if (!programObject) {
		cerr << "Program couldn't be created" << endl;
		return;
	}

	glAttachShader(programObject, vertexShader->id());
	glAttachShader(programObject, fragmentShader->id());

	glBindAttribLocation(programObject, 0, "vPosition");

	glLinkProgram(programObject);

	GLint linked = 0;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		cerr << "Not linked!" << endl;
		glDeleteProgram(programObject);
	}
}

void ShaderProgram::activate() {
	glUseProgram(this->id());
}

void ShaderProgram::setMatrixValue(const char * variableName, glm::mat4 matrix) {
	int varLoc = glGetUniformLocation(this->id(), variableName);
	glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

GLint ShaderProgram::id() {
	return this->programId;
}


