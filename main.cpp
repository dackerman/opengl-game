/*
 * main.c
 *
 *  Created on: Apr 15, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include "GL/glew.h"
#include "GL/glfw.h"

#include <stdio.h>
#include <stdlib.h>

int openWindow(int width, int height) {

	if (!glfwInit()) {
		printf("Init failed!");
		return 0;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

	glfwSetWindowTitle("");
	if (!glfwOpenWindow(width, height, 8, 8, 8, 8, 8, 0, GLFW_WINDOW)) {
		printf("Couldn't open winda!");
		return 0;
	}
	glfwSetWindowTitle("My winda");
	return 1;
}

int compileShader(GLenum shaderType, const char* sourceCode) {
	GLint compiled;

	GLuint shader = glCreateShader(shaderType);

	if (!shader) {
		printf("Couldn't create shader");
		return 0;
	}

	glShaderSource(shader, 1, &sourceCode, NULL);

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
		return 0;
	}

	return 1;
}

int update() {
	return 1;
}

int main(int argc, char* argv[]) {
	if (!openWindow(640, 480)) {
		printf("Couldn't open window!");
		return 1;
	}
	printf("Opened winda!");

	GLfloat points[] = { -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 1.0f };

	const GLchar vShaderStr[] =
			"attribute vec4 vPosition; \n"
			"void main() {\n"
			"gl_Position = vPosition; \n"
			"}\n";

	const GLchar fShaderStr[] =
			"precision mediump float; \n"
			"void main() {\n"
			"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
			"}\n";

	GLint linked;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vShaderStr);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderStr);

	GLuint programObject = glCreateProgram();

	if (!programObject) {
		printf("Program couldn't be created\n");
		return 0;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	glBindAttribLocation(programObject, 0, "vPosition");

	glLinkProgram(programObject);

	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		printf("Not linked!\n");
		glDeleteProgram(programObject);
		return 0;
	}

	int running = 1;
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, points);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC);
	}
	glfwTerminate();
	return 0;
}
