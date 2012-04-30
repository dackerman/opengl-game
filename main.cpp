/*
 * main.c
 *
 *  Created on: Apr 15, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include <GL/glew.h>
#include <GL/glfw.h>

// GLM Matrix calculations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "shader.hpp"
#include "ShaderProgram.hpp"

using namespace std;

int openWindow(int width, int height) {
	if (!glfwInit()) {
		cerr << "Init failed!" << endl;
		return 0;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);

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
	std::vector<unsigned int> faces;
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

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glm::mat4 projectionMatrix;
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 500.0f);
}

int renderMain(mesh * renderMesh) {
	if (!openWindow(640, 480)) {
		cerr << "Couldn't open window!";
		return 1;
	}

	Shader * vertexShader = new Shader(GL_VERTEX_SHADER, "../assets/shaders/simple.vert");
	Shader * fragmentShader = new Shader(GL_FRAGMENT_SHADER, "../assets/shaders/simple.frag");

	ShaderProgram * program = new ShaderProgram(vertexShader, fragmentShader);
	program->link();

//	GLuint programObject = glCreateProgram();
//
//	if (!programObject) {
//		cerr << "Program couldn't be created" << endl;
//		return 0;
//	}
//
//	glAttachShader(programObject, vertexShader->id());
//	glAttachShader(programObject, fragmentShader->id());
//
//	glBindAttribLocation(programObject, 0, "vPosition");
//
//	glLinkProgram(programObject);
//
//	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
//
//	if (!linked) {
//		cerr << "Not linked!" << endl;
//		glDeleteProgram(programObject);
//		return 0;
//	}

	int running = 1;
	cout << "Starting game loop" << endl;

	// Vertex Buffer Objects
	GLuint pointsVBO;
	glGenBuffers(1, &pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, renderMesh->vertices.size() * sizeof(float), renderMesh->vertices.data(), GL_STATIC_DRAW);

	GLuint indicesVBO;
	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderMesh->faces.size() * sizeof(int), renderMesh->faces.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	// Vertex Buffer Objects

	// View Matrix Calculations
	int w = 640;
	int h = 480;
	glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)w / (float) h, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 modelMatrix = glm::mat4(1.0f);//glm::rotate(glm::mat4(1.0f), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 modelViewMatrix;
	// View Matrix Calculations

	while (running) {
		modelViewMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 1.0f, 0.3f, 1.0f);

		program->activate();
		program->setMatrixValue("modelViewMatrix", modelViewMatrix);

		glDrawElements(GL_TRIANGLES, renderMesh->faces.size(), GL_UNSIGNED_INT, 0);

		glGetError();

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC);

		if (glfwGetKey(GLFW_KEY_UP)) {
			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.01f, 0.0f));
		}
		if (glfwGetKey(GLFW_KEY_DOWN)) {
			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, -0.01f, 0.0f));
		}
		if (glfwGetKey(GLFW_KEY_LEFT)) {
			viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.01f, 0.0f, 0.0f));
		}
		if (glfwGetKey(GLFW_KEY_RIGHT)) {
			viewMatrix = glm::translate(viewMatrix, glm::vec3(0.01f, 0.0f, 0.0f));
		}

	}
	cout << "Exited game loop" << endl;
	glfwTerminate();
	return 0;
}

mesh buildCube() {
	const GLfloat vertices[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
	};

	GLuint indices[] = {
			0, 1, 2,
			2, 3, 0,

			3, 2, 6,
			6, 7, 3,

			0, 3, 7,
			7, 4, 0,

			1, 6, 2,
			1, 5, 6,

			0, 4, 1,
			1, 4, 5,

			7, 6, 5,
			5, 4, 7
	};
	mesh cubeMesh;
	cubeMesh.vertices.assign(vertices, vertices + 24);
	cubeMesh.faces.assign(indices, indices + 36);
	return cubeMesh;
}

int main(int argc, char* argv[]) {
	mesh mymesh;
	readObjFile("../assets/duck-tris.obj", &mymesh);
	//mesh mymesh = buildCube();
	cout << "vertices length is " << mymesh.vertices.size() << endl;
	cout << "faces    length is " << mymesh.faces.size() << endl;
	renderMain(&mymesh);
}
