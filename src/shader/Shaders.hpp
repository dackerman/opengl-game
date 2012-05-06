/*
 * Shaders.hpp
 *
 *  Created on: May 6, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef SHADERS_HPP_
#define SHADERS_HPP_

#include <string>

using namespace std;

class Shaders {
public:
	ShaderProgram * getShader(string vertexShader, string fragmentShader);

private:

};


#endif /* SHADERS_HPP_ */
