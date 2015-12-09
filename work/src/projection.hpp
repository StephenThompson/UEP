#pragma once
#include "mainheaders.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <vector>


class Projection{
private:
	std::vector<std::vector<float>> noise_map;
	std::vector<std::vector<std::vector<float>>> noise_map3D;
	int seed;
	int width;
	int height;
	int depth;
	float time;
	float dt = 0.05;
	float tx=0;
	float ty=0;

	float interpolation(float, float, float);
	float cubicInterpolation(float, float, float, float, float);

	float smoothNoise(float);
	float perlinNoise(float);
	float noise(int);

	float interpolation2D(float, float, float);
	float interpolateNoise2D(float,float);
	float smoothNoise2D(float,float);
	float perlinNoise2D(float,float);
	float noise2D(int,int);

	float interpolation3D(float, float, float);
	float interpolateNoise3D(float,float,float);
	float smoothNoise3D(float,float,float);
	float perlinNoise3D(float,float,float);
	float noise3D(int,int,int);
public:
	Projection(int);
	void test();
	void renderLandscape();
	void testTexture();
	void setTranslation(float,float);
	float getHeight(float,float);
	std::vector<std::vector<float>> getHeightMap();
};
