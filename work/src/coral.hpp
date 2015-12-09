/*
 * coral.hpp
 *
 *  Created on: 23/09/2015
 *      Author: gillcall1
 */


#include <iostream>
#include <string>
#include <vector>

struct rule {
	char key;
	std::string replace;

};

class Coral{

private:

	GLuint m_displayList = 0;

	
	float baseX;
float baseY;
float baseZ;

float r;
float g;
float b;

float baseAngle = 30;
float angleOffset = 10;
float branchSize = 0.5f;
float branchSizeOffset = 0.15f;
float lineSizeDecrease = 0.7f;
float lineSize = 0.4;
float m_seed = 0.0f;
float minWidth = 0.05;
std::string commands = "";
std::vector<rule> rules;

	void drawLine();
	void pitchUp();
	void pitchDown();
	void yawL();
	void yawR();
	void rollL();
	void rollR();
	void pop();
	void push();
	void Cylinder(float length, float x, float y, float z, float base, float tip, int slices);
	float noise(int x);

//	void renderBranch(branch*, GLUquadric*);
public:
//	void renderCoral();
	Coral(std::string, float seed, float x, float y, float z, float scale, float colR, float colG, float colB);
	void generateCoral(float angle);
	void grow();
	void draw();
	bool addRule(char key, std::string value);

};
