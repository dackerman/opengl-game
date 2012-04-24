/*
 * main.c
 *
 *  Created on: Apr 15, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include <GL/glew.h>
#include <GL/glfw.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int openWindow(int width, int height) {
	if (!glfwInit()) {
		cerr << "Init failed!" << endl;
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

	int err = glewInit();
	const char* errorString = (char*) glewGetErrorString(err);
	if (GLEW_OK != err) {
		cerr << errorString << endl;
		return 0;
	}

	return 1;
}

int compileShader(GLenum shaderType, const char* sourceCode) {
	GLint compiled;

	GLuint shader = glCreateShader(shaderType);

	if (!shader) {
		cerr << "Couldn't create shader" << endl;
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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

typedef struct {
	std::vector<float> vertices;
	std::vector<int> faces;
} mesh;

void readObjFile(const char* filename, mesh * outmesh) {
	ifstream file;
	file.open(filename, fstream::in);
	if (!file) {
		cerr << "Couldn't open file!" << endl;
		exit(1);
	}

	string lineread;
	while (std::getline(file, lineread)) {
		std::vector<std::string> sline = split(lineread, ' ');
		if ("v" == sline[0]) {
			float x = atof(sline[1].c_str());
			float y = atof(sline[2].c_str());
			float z = atof(sline[3].c_str());
			outmesh->vertices.push_back(x);
			outmesh->vertices.push_back(y);
			outmesh->vertices.push_back(z);
		} else if ("f" == sline[0]) {
			int vx = atoi(sline[1].c_str());
			int vy = atoi(sline[2].c_str());
			int vz = atoi(sline[3].c_str());
			outmesh->faces.push_back(vx);
			outmesh->faces.push_back(vy);
			outmesh->faces.push_back(vz);
		}
	}
	file.close();
}

int renderMain(mesh * renderMesh) {
	if (!openWindow(640, 480)) {
		cerr << "Couldn't open window!";
		return 1;
	}

	const GLchar vShaderStr[] =
			"attribute vec4 vPosition; \n"
			"void main() {\n"
			"gl_Position = vPosition; \n"
			"}\n";

	const GLchar fShaderStr[] =
			"precision mediump float; \n"
			"void main() {\n"
			"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
			"} \n";

	GLint linked;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vShaderStr);
	if (!vertexShader) {
		cerr << "Couldn't build vertex shader" << endl;
		return 1;
	}

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fShaderStr);
	if (!fragmentShader) {
		cerr << "Couldn't build fragment shader" << endl;
		return 1;
	}

	GLuint programObject = glCreateProgram();

	if (!programObject) {
		cerr << "Program couldn't be created" << endl;
		return 0;
	}

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	glBindAttribLocation(programObject, 0, "vPosition");

	glLinkProgram(programObject);

	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		cerr << "Not linked!" << endl;
		glDeleteProgram(programObject);
		return 0;
	}

	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	int running = 1;
	cout << "Starting game loop" << endl;
	while (running) {
		glViewport(0, 0, 640, 480);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programObject);
		glDisable(GL_CULL_FACE);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, renderMesh->vertices.data());
		glDrawElements(GL_TRIANGLES, renderMesh->faces.size(), GL_UNSIGNED_BYTE, renderMesh->faces.data());

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC);
	}
	cout << "Exited game loop" << endl;
	glfwTerminate();
	return 0;
}

int main(int argc, char* argv[]) {
	mesh mymesh;
	readObjFile("../assets/duck-tris.obj", &mymesh);
	cout << "vertices length is " << mymesh.vertices.size() << endl;
	cout << "faces    length is " << mymesh.faces.size() << endl;
	renderMain(&mymesh);
}
