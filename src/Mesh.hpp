/*
 * Mesh.hpp
 *
 *  Created on: May 6, 2012
 *      Author: "David Ackerman (david.w.ackerman@gmail.com)"
 */

#ifndef MESH_HPP_
#define MESH_HPP_

struct mesh;

class Mesh {
public:
	Mesh();
	void load();

private:
	mesh * model;
};

#endif /* MESH_HPP_ */
