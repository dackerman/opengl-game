/*
 * DackGLImporter.hpp
 *
 *  Created on: May 5, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef DACKGLIMPORTER_HPP_
#define DACKGLIMPORTER_HPP_

#include <string>

#include "structs.hpp"

using namespace std;

class DackGLImporter {

public:
	DackGLImporter(string baseDirectory);
	mesh * import(string filename);

private:
	string baseDirectory;
	mesh * model;

	enum Mode { VERTEX, FACE, VERTEX_GROUP, BONE, UNKNOWN };

	bool changeModes(const vector<string> &line, Mode &mode);

	void parseVertex(const vector<string> &line, uint counter);
	void parseFace(const vector<string> &line, uint counter);
	void parseVertexGroup(const vector<string> &line, uint counter);
	void parseBone(const vector<string> &line, uint counter);
};

#endif /* DACKGLIMPORTER_HPP_ */
