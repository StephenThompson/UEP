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

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "mainheaders.hpp"
#include "geometry.hpp"

using namespace std;
using namespace glm;

Geometry::Geometry(string filename, GLuint tex, GLuint shader, bool vertical) {
	m_filename = filename;
	readOBJ(filename);
	m_texture = tex;
	m_shader = shader;
	m_vertical = vertical;
	if (m_triangles.size() > 0) {
		createDisplayList();
	}
}

void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,1,0));

	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;

	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {

				vector<vertex> verts;
				while (objLine.good()){
					vertex v;

					// Assignment code (assumes you have all of v/vt/vn for each vertex)
					objLine >> v.p;		// Scan in position index
					if (m_uvs.size() <= 1 && m_normals.size() <= 1){
						v.n = v.p;
						verts.push_back(v);
						continue;
					}

					objLine.ignore(1);	// Ignore the '/' character

					// Deal with uvs
					if (m_uvs.size() <= 1){
						v.t = 0;
					} else {
						objLine >> v.t;		// Scan in uv (texture coord) index
					}

					objLine.ignore(1);	// Ignore the '/' character

					// Deal the normals
					if (m_normals.size() <= 1){
						v.n = v.p;
					} else {
						objLine >> v.n;		// Scan in normal index
					}
					verts.push_back(v);
				}

				// IFF we have 3 verticies, construct a triangle
				if(verts.size() == 3){
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;


	// If we didn't have any normals, create them
	if (m_normals.size() <= 1)
		createNormals();
}
/*
void Geometry::initMaterial(vec3 ambient, vec3 diffuse, vec3 specular, float shininess){
	float amb[] = {ambient.x, ambient.y, ambient.z};
	float dif[] = {diffuse.x, diffuse.y, diffuse.z};
	float spec[] = {specular.x, specular.y, specular.z};
	float emit[] = { 0, 0, 0};

	glMaterialfv(GL_FRONT, GL_EMISSION, emit);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
*/
// Generates normals for the model
//
void Geometry::createNormals() {
	m_normals.clear();

	for (int k = 0; k < m_points.size(); ++k){
		m_normals.push_back(vec3(0.f, 0.f, 0.f));
	}

	for (int k = 0; k < m_triangles.size(); ++k){
		triangle t = m_triangles[k];

		// Get Normal
		vec3 v1 = m_points[t.v[0].n] - m_points[t.v[1].n];
		vec3 v2 = m_points[t.v[0].n] - m_points[t.v[2].n];
		vec3 normal = cross(v1, v2);

		for (int i = 0; i < 3; ++i){
			m_normals[t.v[i].n] += normal;
		}
	}

	for (int k = 1; k < m_normals.size(); ++k){
		m_normals[k] = normalize(m_normals[k]);
	}
}

// Creates a display list to show a filled model
//
void Geometry::createDisplayList() {
	// Delete old list if there is one
	if (m_displayList) glDeleteLists(m_displayList, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayList = glGenLists(1);
	glNewList(m_displayList, GL_COMPILE);

	// Adds geometry
	glBegin(GL_TRIANGLES);

	for (int k = 0; k < m_triangles.size(); ++k){
		triangle t = m_triangles[k];
		for (int i = 0; i < 3; ++i){
			glNormal3f(m_normals[t.v[i].n].x, m_normals[t.v[i].n].y, m_normals[t.v[i].n].z);
			glTexCoord2f(m_uvs[t.v[i].t].x, m_uvs[t.v[i].t].y);
			glVertex3f(m_points[t.v[i].p].x, m_points[t.v[i].p].y, m_points[t.v[i].p].z);
		}
	}

	glEnd();
	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}


void Geometry::renderGeometry(float time) {


	// Set the location for binding the texture
	//glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Use the shader we made
	glUseProgram(m_shader);

	glUniform1f(glGetUniformLocation(m_shader, "vertical"), m_vertical);
	glUniform1f(glGetUniformLocation(m_shader, "time"), time);
	glUniform1f(glGetUniformLocation(m_shader, "amount"), 0.5f);

	// Set our sampler (texture0) to use GL_TEXTURE0 as the source
	glUniform1i(glGetUniformLocation(m_shader, "texture0"), 0);

	glCallList(m_displayList);
}

