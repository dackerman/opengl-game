/*
 * shader.cpp
 *
 *  Created on: Apr 29, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include "shader.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>

using namespace std;

Shader::Shader(GLenum shaderType, const char * filename) {
	this->type = shaderType;
	this->filename = filename;
	this->compiledId = 0;
}

void Shader::compile() {
	GLint compiled;

	const char * source = this->getShader(this->filename);

	int shader = glCreateShader(this->type);

	if (!shader) {
		cerr << "couldn't create shader" << endl;
		throw std::exception();
	}

	glShaderSource(shader, 1, &source, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*) malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

			printf("Error compiling shader:\n%s\n", infoLog);

			free(infoLog);
		}

		glDeleteShader(shader);
		cerr << "failed to compile shader" << endl;
		throw std::exception();
	}
	free((void*)source);
	this->compiledId = shader;
}

GLuint Shader::id() {
	if (!this->compiledId) {
		cerr << "shader wasn't compiled!" << endl;
		throw std::exception();
	}
	return this->compiledId;
}

const char * Shader::getShader(const char * filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		cerr << "couldn't open file " << filename << endl;
		exit(1);
	}
	if(fseek(file, 0, SEEK_END)) {
		fclose(file);
		cerr << "couldn't seek in file " << filename << endl;
		exit(1);
	}

	int size = ftell(file);
	if (size < 0) {
		fclose(file);
		cerr << "couldn't get position in file " << filename << endl;
		exit(1);
	}

	char * text = (char*) malloc(size + 1);
	if (!text) {
		fclose(file);
		cerr << "couldn't allocate memory for text file " << filename << endl;
		exit(1);
	}
	memset(text, 0, size + 1);
	rewind(file);

	fread(text, 1, size, file);

	fclose(file);
	return text;
}

