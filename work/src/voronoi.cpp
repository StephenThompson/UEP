#include "mainheaders.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include "voronoi.hpp"


using namespace std;
using namespace glm;

Voronoi::Voronoi(){
	CreatePoints();
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,500,500,0,GL_RGB,GL_UNSIGNED_BYTE,0);


//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	glOrtho(0,10,0,10,1,-1);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
}

void Voronoi::render(){
	for (int i = 0; i < points.size();i++){
		points[i].x	+= ((rand()%100)-50)*speed;
		points[i].y += ((rand()%100)-50)*speed;
		if (points[i].x > 2) points[i].x = 2;
		if (points[i].y > 2) points[i].y = 2;
		if (points[i].x < -2) points[i].x = -2;
		if (points[i].y < -2) points[i].y = -2;
	}
	CreateSites();
}

void Voronoi::CreatePoints(){
	float xSpacing = 2.0/20;
	float ySpacing = 2.0/20;

	glBegin(GL_POINTS);

	for (int i = 0; i < 100; i++) {
		float x = (rand()%40)*xSpacing-2.0;
		float y = (rand()%40)*ySpacing-2.0;
		points.push_back({x,y});
	}

	glEnd();
}

void Voronoi::CreateSites(){
  	glViewport(0, 0, 600, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-2,2,-2,2,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	float radius = 2;
	float ratio = 1;
	float color = 1;
	for (int p = 0; p < points.size();p++){
		glColor4f(color,color,color,1);
		color-=1/float(points.size());
		glBegin(GL_TRIANGLES);
		for(int i = 0; i < 20; i++)	{
			float theta = 2.0f * 3.1415926f * float(i) / float(20);//get the current angle
			//
			float rad_x = ratio*(radius * cosf(theta));//calculate the x component
			float rad_y = radius * sinf(theta);//calculate the y component
			//
			glColor3f(1,1,1);
			glVertex3f(points[p].x + rad_x, points[p].y + rad_y,0);//output vertex
			glColor3f(0.2,0.2,0.2);
			glVertex3f(points[p].x, points[p].y,0.5);

			theta = 2.0f * 3.1415926f * float(i+1) / float(20);//get the current angle
			//
			rad_x = ratio*(radius * cosf(theta));//calculate the x component
			rad_y = radius * sinf(theta);//calculate the y component
			glColor3f(1,1,1);
			glVertex3f(points[p].x + rad_x, points[p].y + rad_y,0);//output vertex

		}
		glEnd();
	}
	CreateTextures();
}

void Voronoi::CreateTextures(){
	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 500, 500, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnd();
}

void Voronoi::increaseSpeed(){
	speed*=1.1;
}
void Voronoi::decreaseSpeed(){
	speed*=0.9;
}
