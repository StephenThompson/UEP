#pragma once
#include "mainheaders.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


class Projection{
private:
	std::vector<std::vector<float>> noise_map;
	std::vector<std::vector<float>> heights;
	std::vector<std::vector<float>> weights;
	std::vector<std::vector<std::vector<float>>> noise_map3D;
	GLuint m_displaylist = 0;
	int seed;
	int width;
	int height;

	float scale;
	float bumpiness;

	int depth;
	int trim = 10;
	float time;
	float dt = 0.05;
	float tx=0;
	float ty=0;


	float random();
	void addHills();
	void smoothPoints();
public:
	Projection();
	void setWeights(int);
	void renderLandscape();
	float getHeight(float,float);
};
