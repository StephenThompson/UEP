/*
 * coral.cpp
 *
 *  Created on: 23/09/2015
 *      Author: gillcall1
 */

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <GLFW/glfw3.h>

#include "coral.hpp"

using namespace std;


Coral::Coral(string s, float seed, float x, float y, float z, float scale, float colR, float colG, float colB){
	commands = s;
	m_seed = seed;
	baseX=x;
	baseY=y;
	baseZ=z;
	r=colR;
	g=colG;
	b=colB;
	branchSize = branchSize*scale;
	branchSizeOffset = branchSizeOffset*scale;
	lineSize = lineSize*scale;
}

void Coral::drawLine(){
	float length = (noise(m_seed)*branchSizeOffset)+branchSize;
	glLineWidth(lineSize);
	glColor3i(230, 230, 0);
	Cylinder(length, 0, 0, 0, lineSize, lineSize, 5);
	glTranslatef(0, 0, length);
}

void Coral::Cylinder(float length, float x, float y, float z, float base, float top, int slices){
	float radiusBase = base/2;
	float radiusTop = top/2;
	glBegin(GL_TRIANGLES);
	for(int i=0; i<slices; i++) {
		float theta = ((float)i/slices)*2.0*3.1459;
		float nextTheta = (((float)i + 1) / slices)*2.0*3.1459;
		/*vertex at middle of end */
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusTop*cos(theta),radiusTop*sin(theta),length);
		/*vertices at edges of circle*/
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f(radiusTop*cos(nextTheta), radiusTop*sin(nextTheta), length);
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f (radiusTop*cos(nextTheta), radiusTop*sin(nextTheta), 0);
		/* the same vertices at the bottom of the cylinder*/
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusTop*cos(theta),radiusTop*sin(theta),length);
		/*vertices at edges of circle*/
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusTop*cos(theta), radiusTop*sin(theta), 0);
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f (radiusTop*cos(nextTheta), radiusTop*sin(nextTheta), 0);

	}
	glEnd();
}

float Coral::noise(int x){
	x+=m_seed;
	m_seed+=1;
	x = (x<<13)^x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

void Coral::pitchUp(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(angle, 1, 0, 0);
}


void Coral::pitchDown(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(-angle, 1, 0, 0);
}

void Coral::yawR(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(angle, 0, 1, 0);
}

void Coral::yawL(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(-angle, 0, 1, 0);
}

void Coral::rollL(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(angle, 0, 0, 1);
}

void Coral::rollR(){
	float angle = (noise(m_seed)*angleOffset)+baseAngle;
	glRotatef(-angle, 0, 0, 1);
}

void Coral::push(){
	glPushMatrix();
//		if (lineSize > 0)
//			lineSize -= lineSizeDecrease;
}

void Coral::pop(){
	glPopMatrix();
//		lineSize += lineSizeDecrease;
}

void Coral::generateCoral(float angle){
	if (m_displayList)
		glDeleteLists(m_displayList, 1);
	m_displayList = glGenLists(1);
	glNewList(m_displayList, GL_COMPILE);
	glEnable(GL_LIGHTING);

	int i = 0;
	float scale = 1.0f;
	glTranslatef(baseX, baseY, baseZ);
//	glScalef(0.1f, 0.1f, 0.1f);
	glRotatef(-90, 1, 0, 0);
//	GLUquadric *quad = gluNewQuadric();
	while(i<commands.length()){
		switch(commands.at(i)){
			case 'D':
				drawLine();
				break;
			case '>':
				yawR();
				break;
			case '<':
				yawL();
				break;
			case '^':
				pitchUp();
				break;
			case '_':
				pitchDown();
				break;
			case '+':
				rollR();
				break;
			case '-':
				rollL();
				break;
			case '[':
				push();
				break;
			case ']':
				pop();
				break;
			case '#':
				lineSize*=lineSizeDecrease;
				if(lineSize<minWidth){
				 lineSize = minWidth;
				}
				break;
			case '&':
				lineSize/=lineSizeDecrease;
				break;
			default:
				break;
		}
		i++;
	}
	glEndList();
}

void Coral::draw(){
	glShadeModel(GL_SMOOTH);
	//glColor3f(r,g,b);
	float col[3] = {r,g,b};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glCallList(m_displayList);
}

void Coral::grow(){
	string newCommands = "";
	for(int i=0; i<commands.length(); i++){
		bool swapped = false;
		for(rule r : rules){
			if(commands.at(i)==r.key){
				newCommands+=r.replace;
				swapped = true;
			}
		}
		if(!swapped){
			newCommands+=commands.at(i);
		}
	}
	commands=newCommands;
}

bool Coral::addRule(char key, string val){
	rule r;
	r.key=key;
	r.replace=val;
	rules.push_back(r);
	return true;
}

//void Coral::renderBranch(branch *b, GLUquadric* q) {
//	// YOUR CODE GOES HERE
//	float T = 0.05;
//
//	//Transforms
//	glPushMatrix();
//
//	glRotatef(b->rotation.z, 0, 0, 1);
//	glRotatef(b->rotation.y, 0, 1, 0);
//	glRotatef(b->rotation.x, 1, 0, 0);
////	glColor3f(0,1,1);
////	glutSolidSphere(b->radiusBase, 100, 100);
//	glColor3f(0.8,0.8,0.8);
//	gluCylinder(q,b->radiusBase,b->radiusTip,b->length,10,10);
//	//Translate along bone length
//	glTranslatef(0, 0, b->length);
//
////	for(branch* c: b->children){
////		renderBranch(c,q);
////	}
//
//	glPopMatrix();
//	// ...
//}


