/*
 * utils.hpp
 *
 *  Created on: May 5, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>

#include "structs.hpp"

using namespace std;

namespace utils {

vector<string> &split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim) {
	vector<string> elems;
	return split(s, delim, elems);
}

void readDackFile(const char* filename, oldmesh * outmesh) {
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

void readObjFile(const char* filename, oldmesh * outmesh) {
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
			vertexnormal vn = {x, y, z, 0.f, 0.f, 0.f};
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
}

#endif /* UTILS_HPP_ */
