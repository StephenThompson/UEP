#include "mainheaders.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


struct Point {
  float x, y;
};
class Voronoi{
private:
	int size = 640;
	float speed = 0.0018f;
	std::vector<Point> points;
	float* data = new float[size*size*3];
	std::vector<GLuint> texMap;
	GLuint tex;

	void SetSitesPoints();
	void CreatePoints();
	void CreateSites();
	void CreateTextures();

public:
	Voronoi();
	void render();
	void increaseSpeed();
	void decreaseSpeed();
};
