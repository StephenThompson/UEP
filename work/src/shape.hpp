/*
 * shape.hpp
 *
 *  Created on: 15/10/2015
 *      Author: deightcull
 */

#include <vector>
#include <cmath>
#include "mainheaders.hpp"

using namespace glm;
using namespace std;

class Shape{
public:
	Shape(vec3 position, float radius, bool hollow);//Creates a shphere or dome
	Shape(vec3 position, vec3 size, bool hollow);//Creates rectangle
	vec3 randPoint();
private:
	bool m_hollow = false;
	vec3 m_position;
	int seed = 200;
	vec3 m_size = vec3(-1, -1, -1);
	float m_radius=-1;
	float noise(int x);
};
