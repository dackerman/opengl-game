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
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "ShaderProgram.hpp"

using namespace std;

void openWindow(int width, int height) {
	if (!glfwInit()) {
		throw runtime_error("Init failed!");
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);

	glfwSetWindowTitle("");
	if (!glfwOpenWindow(width, height, 8, 8, 8, 8, 8, 0, GLFW_WINDOW)) {
		throw runtime_error("Couldn't init window!");
	}
	glfwSetWindowTitle("Dackgl Game Engine v0.01");

	int err = glewInit();
	const char* errorString = (char*) glewGetErrorString(err);
	if (GLEW_OK != err) {
		cerr << "couldn't init GLEW: "<< errorString << endl;
		throw runtime_error("out of range");
	}
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    return split(s, delim, elems);
}

struct vertexnormal {
	float vx;
	float vy;
	float vz;
	float nx;
	float ny;
	float nz;
};

typedef struct {
	glm::vec3 direction;
	glm::vec3 halfplane;
	glm::vec4 ambient_color;
	glm::vec4 diffuse_color;
	glm::vec4 specular_color;
} directional_light;

struct material_props {
	glm::vec4 ambient_color;
	glm::vec4 diffuse_color;
	glm::vec4 specular_color;
	float specular_exponent;
};

typedef struct {
	string name;
	glm::mat4x3 inverseBindPose;
	int8_t parent;
} bone;

typedef struct {
	vector<vertexnormal> vertices;
	vector<float> normals;
	vector<unsigned int> faces;

	vector<bone> bones;

	material_props material;
} mesh;

void readDackFile(const char* filename, mesh * outmesh) {
	ifstream file;
	file.open(filename, fstream::in);
	if (!file) {
		throw runtime_error("Couldn't open file!");
	}

	string lineread;
	while (getline(file, lineread)) {
		vector<string> sline = split(lineread, ' ');
		bone b;
		b.name = sline[0];
		glm::mat3 rest;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				rest[i][j] = atof(sline[i * 3 + j + 1].data());
			}
		}
		//b.inverseBindPose = rest;
		outmesh->bones.push_back(b);
	}
}

void readObjFile(const char* filename, mesh * outmesh) {
	ifstream file;
	file.open(filename, fstream::in);
	if (!file) {
		throw runtime_error("Couldn't open file!");
	}

	string lineread;
	while (getline(file, lineread)) {
		vector<string> sline = split(lineread, ' ');
		if ("vn" == sline[0]) {
			float x = atof(sline[1].c_str());
			float y = atof(sline[2].c_str());
			float z = atof(sline[3].c_str());
			outmesh->normals.push_back(x);
			outmesh->normals.push_back(y);
			outmesh->normals.push_back(z);
		} else if ("v" == sline[0]) {
			float x = atof(sline[1].c_str());
			float y = atof(sline[2].c_str());
			float z = atof(sline[3].c_str());
			struct vertexnormal vn = {x, y, z, 0.f, 0.f, 0.f};
			outmesh->vertices.push_back(vn);
		} else if ("f" == sline[0]) {
			vector<string> sv1 = split(sline[1], '/');
			vector<string> sv2 = split(sline[2], '/');
			vector<string> sv3 = split(sline[3], '/');

			int v1 = atoi(sv1[0].c_str()) - 1;
			int v2 = atoi(sv2[0].c_str()) - 1;
			int v3 = atoi(sv3[0].c_str()) - 1;

			int vn1 = atoi(sv1[2].c_str()) - 1;
			int vn2 = atoi(sv2[2].c_str()) - 1;
			int vn3 = atoi(sv3[2].c_str()) - 1;

			outmesh->vertices[v1].nx = outmesh->normals[vn1 * 3];
			outmesh->vertices[v1].ny = outmesh->normals[vn1 * 3 + 1];
			outmesh->vertices[v1].nz = outmesh->normals[vn1 * 3 + 2];

			outmesh->vertices[v2].nx = outmesh->normals[vn2 * 3];
			outmesh->vertices[v2].ny = outmesh->normals[vn2 * 3 + 1];
			outmesh->vertices[v2].nz = outmesh->normals[vn2 * 3 + 2];

			outmesh->vertices[v3].nx = outmesh->normals[vn3 * 3];
			outmesh->vertices[v3].ny = outmesh->normals[vn3 * 3 + 1];
			outmesh->vertices[v3].nz = outmesh->normals[vn3 * 3 + 2];

			outmesh->faces.push_back(v1);
			outmesh->faces.push_back(v2);
			outmesh->faces.push_back(v3);
		}
	}
	file.close();
	cout << "Loaded file " << filename << endl;
	cout << "    vertices: " << outmesh->vertices.size() << endl;
	cout << "    normals:  " << outmesh->normals.size() << endl;
	cout << "    faces:    " << outmesh->faces.size() << endl;
}

void renderMain(mesh * renderMesh) {

	directional_light light;
	light.ambient_color = glm::vec4(0.5f);
	light.diffuse_color = glm::vec4(0.5f);
	light.specular_color = glm::vec4(0.5f);

	Shader * vertexShader = new Shader(GL_VERTEX_SHADER, "../assets/shaders/lighted.vert");
	Shader * fragmentShader = new Shader(GL_FRAGMENT_SHADER, "../assets/shaders/simple.frag");

	ShaderProgram * program = new ShaderProgram(vertexShader, fragmentShader);
	program->link();

	int running = 1;

	// Vertex Buffer Objects
	GLuint pointsVBO;
	glGenBuffers(1, &pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, renderMesh->vertices.size() * sizeof(vertexnormal), renderMesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexnormal), 0);
	glEnableVertexAttribArray(0);

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, renderMesh->vertices.size() * sizeof(vertexnormal), renderMesh->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertexnormal), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint indicesVBO;
	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderMesh->faces.size() * sizeof(int), renderMesh->faces.data(), GL_STATIC_DRAW);

	// Vertex Buffer Objects

	// View Matrix Calculations
	int w = 640;
	int h = 480;
	glm::mat4 projectionMatrix = glm::perspective(45.0f, (float)w / (float) h, 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	glm::mat4 modelViewMatrix;
	// View Matrix Calculations

	glEnable(GL_DEPTH_TEST);

	glm::vec3 translate;

	float rotateX = 0.0f;
	float rotateY = 0.0f;
	float rotateZ = 0.0f;

	while (running) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::rotate(modelMatrix, rotateX, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotateY, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotateZ, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::translate(glm::mat4(1.0f), translate) * modelMatrix;

		glm::vec4 pos4 = glm::inverse(modelMatrix) * glm::inverse(viewMatrix) * glm::normalize(glm::vec4(0.0f, 1.0f, 1.0f, 0.0f));
		light.direction.x = pos4.x;
		light.direction.y = pos4.y;
		light.direction.z = pos4.z;
		light.halfplane = glm::normalize(light.direction + glm::vec3(0.0f, 0.0f, 1.0f)); // || Plight + (0, 0, 1) ||

		modelViewMatrix = projectionMatrix * viewMatrix * modelMatrix;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.3f, 1.0f, 0.3f, 1.0f);

		program->activate();
		program->setMatrix("modelViewMatrix", modelViewMatrix);

		program->setVector("material.ambient_color", renderMesh->material.ambient_color);
		program->setVector("material.diffuse_color", renderMesh->material.diffuse_color);
		program->setVector("material.specular_color", renderMesh->material.specular_color);
		program->setFloat("material.specular_exponent", renderMesh->material.specular_exponent);

		program->setVector("light.direction", light.direction);
		program->setVector("light.halfplane", light.halfplane);
		program->setVector("light.ambient_color", light.ambient_color);
		program->setVector("light.diffuse_color", light.diffuse_color);
		program->setVector("light.specular_color", light.specular_color);

		glDrawElements(GL_TRIANGLES, renderMesh->faces.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers();

		running = !glfwGetKey(GLFW_KEY_ESC);

		if (glfwGetKey(GLFW_KEY_UP)) {
			rotateY += 1.0f;
		}
		if (glfwGetKey(GLFW_KEY_DOWN)) {
			rotateY -= 1.0f;
		}
		if (glfwGetKey(GLFW_KEY_LEFT)) {
			rotateX -= 1.0f;
		}
		if (glfwGetKey(GLFW_KEY_RIGHT)) {
			rotateX += 1.0f;
		}
		if (glfwGetKey('W')) {
			translate.y += 0.05f;
		}
		if (glfwGetKey('S')) {
			translate.y -= 0.05f;
		}
		if (glfwGetKey('A')) {
			translate.x -= 0.05f;
		}
		if (glfwGetKey('D')) {
			translate.x += 0.05f;
		}

	}
	glfwTerminate();
}

void printMatrix(glm::mat4x3 matrix) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int main(int argc, char* argv[]) {
	mesh mymesh;
	readObjFile("../assets/ragdoll.obj", &mymesh);
	readDackFile("../assets/ragdoll_bones.dack", &mymesh);

	for (unsigned int i = 0; i < mymesh.bones.size(); i++) {
		bone b = mymesh.bones[i];
		cout << "bone: " << b.name << endl;
		printMatrix(b.inverseBindPose);
		cout << endl;
	}

	mymesh.material.ambient_color = glm::vec4(0.2f);
	mymesh.material.diffuse_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mymesh.material.specular_color = glm::vec4(1.0f);
	mymesh.material.specular_exponent = 2.0f;

	openWindow(640, 480);

	renderMain(&mymesh);
}
