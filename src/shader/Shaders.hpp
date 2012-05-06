/*
 * Shaders.hpp
 *
 *  Created on: May 6, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef SHADERS_HPP_
#define SHADERS_HPP_

#include <string>
#include <map>

#include "ShaderProgram.hpp"

using namespace std;

namespace dackgl {
namespace loaders {

class Shaders {
public:

	Shaders(string shaderDirectory) {
		this->shaderDirectory = shaderDirectory;
	}

	ShaderProgram * getShader(string vertexShader, string fragmentShader);

private:
	string shaderDirectory;
	map<string, Shader *> shaders;
	map<string, ShaderProgram *> shaderPrograms;

	Shader * getOrCreateShader(GLenum shaderType, string name);
	string shaderPath(string name);
};

} }

#endif /* SHADERS_HPP_ */
