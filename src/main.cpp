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

#include <stdexcept>
#include <iostream>

#include "shader/Shader.hpp"
#include "shader/ShaderProgram.hpp"
#include "DackGLImporter.hpp"
#include "structs.hpp"

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
	glBufferData(GL_ARRAY_BUFFER, renderMesh->numVertices * sizeof(vertexnormal), renderMesh->vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexnormal), 0);
	glEnableVertexAttribArray(0);

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, renderMesh->numVertices * sizeof(vertexnormal), renderMesh->vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertexnormal), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint indicesVBO;
	glGenBuffers(1, &indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderMesh->numFaces * sizeof(int), renderMesh->faces, GL_STATIC_DRAW);

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

		glDrawElements(GL_TRIANGLES, renderMesh->numFaces, GL_UNSIGNED_INT, 0);

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
	DackGLImporter * importer = new DackGLImporter();
	mesh mymesh = importer->import("../assets/ragdoll.dack");

	mymesh.material.ambient_color = glm::vec4(0.2f);
	mymesh.material.diffuse_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	mymesh.material.specular_color = glm::vec4(1.0f);
	mymesh.material.specular_exponent = 2.0f;

	openWindow(640, 480);

	renderMain(&mymesh);
}
