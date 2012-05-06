/*
 * Shaders.cpp
 *
 *  Created on: May 6, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#include "Shaders.hpp"

#include <iostream>

#include "ShaderProgram.hpp"

namespace dackgl {
namespace loaders {

ShaderProgram * Shaders::getShader(string vertexShaderName, string fragmentShaderName) {
	string programKey = vertexShaderName + ":" + fragmentShaderName;
	if (shaderPrograms.count(programKey) > 0) {
		return shaderPrograms[programKey];
	} else {
		Shader * vertexShader =  getOrCreateShader(GL_VERTEX_SHADER, vertexShaderName);
		Shader * fragmentShader = getOrCreateShader(GL_FRAGMENT_SHADER, fragmentShaderName);
		ShaderProgram * program = new ShaderProgram(vertexShader, fragmentShader);
		shaderPrograms[programKey] = program;
		return program;
	}
}

Shader * Shaders::getOrCreateShader(GLenum shaderType, string name) {
	if (shaders.count(name) > 0) {
		return shaders[name];
	} else {
		Shader * shader;
		if (shaderType == GL_VERTEX_SHADER) {
			string path = shaderPath(name);
			shader = new Shader(GL_VERTEX_SHADER, path.c_str());
		} else {
			string path = shaderPath(name);
			shader = new Shader(GL_FRAGMENT_SHADER, path.c_str());
		}
		shaders[name] = shader;
		return shader;
	}
}

string Shaders::shaderPath(string name) {
	return shaderDirectory + "/" + name;
}

} }


