/*
 * DackGLImporter.hpp
 *
 *  Created on: May 5, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef DACKGLIMPORTER_HPP_
#define DACKGLIMPORTER_HPP_

#include <string>

using namespace std;


class DackGLImporter {

public:
	DackGLImporter(string filename);
	void import();

private:
	string filename;
};

#endif /* DACKGLIMPORTER_HPP_ */
