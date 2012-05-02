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
	glBindAttribLocation(programObject, 1, "vNormal");

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

void ShaderProgram::setMatrix(const char * name, glm::mat4 matrix) {
	glUniformMatrix4fv(this->loc(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setVector(const char * name, glm::vec4 vec) {
	glUniform4fv(this->loc(name), 1, glm::value_ptr(vec));
}

void ShaderProgram::setVector(const char * name, glm::vec3 vec) {
	glUniform3fv(this->loc(name), 1, glm::value_ptr(vec));
}

void ShaderProgram::setFloat(const char * name, float fl) {
	glUniform1f(this->loc(name), fl);
}

int ShaderProgram::loc(const char * name) {
	return glGetUniformLocation(this->id(), name);
}

GLint ShaderProgram::id() {
	return this->programId;
}


