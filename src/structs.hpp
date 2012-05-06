/*
 * Mesh.hpp
 *
 *  Created on: May 5, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef MESH_HPP_
#define MESH_HPP_

#include <glm/glm.hpp>

#include <string>
#include <vector>

using namespace std;

typedef struct {
	float vx;
	float vy;
	float vz;
	float nx;
	float ny;
	float nz;
} vertexnormal;

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
} oldmesh;

typedef struct {
	string name;

	uint numVertices;
	vertexnormal * vertices;

	uint numFaces;
	uint * faces;

	uint numBones;
	bone * bones;

	material_props material;
} mesh;


#endif /* MESH_HPP_ */
