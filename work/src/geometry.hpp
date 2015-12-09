//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "mainheaders.hpp"

struct vertex {
	int p = 0; // index for point in m_points
	int t = 0; // index for uv in m_uvs
	int n = 0; // index for normal in m_normals
};

struct triangle {
	vertex v[3]; //requires 3 verticies
};

class Geometry {
private:

	// Fields for storing raw obj information
	std::string m_filename;
	std::vector<glm::vec3> m_points;	// Point list
	std::vector<glm::vec2> m_uvs;		// Texture Coordinate list
	std::vector<glm::vec3> m_normals;	// Normal list
	std::vector<triangle> m_triangles;		// Triangle/Face list

	// IDs for the display list to render
	GLuint m_displayList = 0; // DisplayList for Polygon
	GLuint m_texture = 0;
	GLuint m_shader = 0;
	bool m_vertical = false;

	void readOBJ(std::string);
	void createNormals();
	void createDisplayList();

public:
	Geometry(std::string, GLuint tex, GLuint shader, bool vertical);
	void renderGeometry(float time);
};
