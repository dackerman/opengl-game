/*
 * DackGLImporter.cpp
 *
 *  Created on: May 5, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "DackGLImporter.hpp"
#include "utils.hpp"

using namespace std;

float tof(string s) {
	return atof(s.data());
}

int toi(string s) {
	return atoi(s.data());
}

DackGLImporter::DackGLImporter(string baseDirectory) {
	this->baseDirectory = baseDirectory;
}

mesh * DackGLImporter::import(string filename) {
	model = new mesh;
	ifstream file;
	string path = baseDirectory + "/" + filename;
	file.open(path.data(), fstream::in);
	if (!file) {
		char * err = new char[200];
		sprintf(err, "Couldn't open file '%s'", filename.data());
		throw runtime_error(err);
	}

	string lineread;
	uint lineNum = 0;
	uint counter = 0;
	Mode mode = UNKNOWN;
	while (getline(file, lineread)) {
		vector<string> line = utils::split(lineread, ' ');
		if (this->changeModes(line, mode)) {
			counter = 0;
			continue;
		}
		switch (mode) {
		case VERTEX:
			this->parseVertex(line, counter); break;
		case VERTEX_GROUP:
			this->parseVertexGroup(line, counter); break;
		case FACE:
			this->parseFace(line, counter); break;
		case BONE:
			this->parseBone(line, counter); break;
		default:
			cerr << "Parser confused! line:" << lineNum;
			cerr << " contents: " << line.data() << endl;
			throw runtime_error("Parser confused!");
			break;
		};
		lineNum++;
		counter++;
	}
	cout << "Imported " << filename.data() << endl;
	cout << " num vertices/normals: " << model->numVertices;
	cout << " num faces: " << model->numFaces << endl << endl;
	return model;
}

bool DackGLImporter::changeModes(const vector<string> &line, Mode &mode) {
	if ("m" == line[0]) {
		model->name = line[1];
		return true;
	} else if ("v" == line[0]){
		mode = VERTEX;
		model->numVertices = atoi(line[1].data());
		model->vertices = new vertexnormal[model->numVertices];
		return true;
	} else if ("f" == line[0]) {
		mode = FACE;
		model->numFaces = atoi(line[1].data()) * 3;
		model->faces = new uint[model->numFaces];
		return true;
	} else if ("g" == line[0]) {
		mode = VERTEX_GROUP;
		return true;
	} else if ("a" == line[0]) {
		return true;
	}
	return false;
}

void DackGLImporter::parseVertex(const vector<string> &line, uint counter) {
	vector<string> vertices = utils::split(line[0], ',');
	model->vertices[counter].vx = tof(vertices[0]);
	model->vertices[counter].vy = tof(vertices[1]);
	model->vertices[counter].vz = tof(vertices[2]);

	vector<string> normals = utils::split(line[1], ',');
	model->vertices[counter].nx = tof(normals[0]);
	model->vertices[counter].ny = tof(normals[1]);
	model->vertices[counter].nz = tof(normals[2]);
}

void DackGLImporter::parseFace(const vector<string> &line, uint counter) {
	model->faces[counter * 3] = toi(line[0]);
	model->faces[counter * 3 + 1] = toi(line[1]);
	model->faces[counter * 3 + 2] = toi(line[2]);
}

void DackGLImporter::parseVertexGroup(const vector<string> &line, uint counter) {
	// TODO(dackerman): Add this in
}

void DackGLImporter::parseBone(const vector<string> &line, uint counter) {
	// TODO(dackerman): Add this in
}
