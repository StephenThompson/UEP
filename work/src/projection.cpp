#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "mainheaders.hpp"
#include "projection.hpp"


using namespace std;

Projection::Projection(int s){
	seed = s;
	time = 0;
	width = 200;
	height = 200;
	depth = 1;

	vector<vector<float>> row;
	vector<float> col;
	//	for (int x = 0; x < width; x++){
	//		for (int y = 0; y < height; y++){
	//			for (int z = 0; z < depth; z++){
	//				col.push_back(perlinNoise3D((x/3.0),(y/3.0),(z/3.0)));
	//			}
	//			row.push_back(col);
	//			col.clear();
	//		}
	//		noise_map3D.push_back(row);
	//		row.clear();
	//	}

	for (int x = 0; x < width; x++){
		for (int z = 0; z < height; z++){
			float temp = perlinNoise2D((x/5.0),(z/5.0));
			//temp *= 3;
			if (temp > 2) temp *= 1.5;
			cout << temp << endl;

			col.push_back(temp);
		}
		noise_map.push_back(col);
		col.clear();
	}

	for(int x = 85; x < 115; x++){
		for (int z = 85; z < 115; z++){
			noise_map[x][z] = 1;
		}
	}
}
void Projection::setTranslation(float x, float y){
	tx = x;
	ty = y;
}
vector<vector<float>> Projection::getHeightMap(){
	return noise_map;
}

float Projection::getHeight(float x, float y){
	/*float dx = 1+fmod(x,1);
	float dy = 1+fmod(y,1);

	float outx = (noise_map[(int)x][(int)y]+noise_map[(int)x+1][(int)y])*dx*0.5-1;
	float outy = (noise_map[(int)x][(int)y]+noise_map[(int)x][(int)y+1])*dy*0.5-1;

	float outx1 = (noise_map[(int)x][(int)y]+noise_map[(int)x+1][(int)y])*dx*0.5-1;
	float outy1 = (noise_map[(int)x][(int)y]+noise_map[(int)x][(int)y+1])*dy*0.5-1;

	return ((((outy+outx)/2.0)*0.9) + (((outy1+outx1)/2.0)*0.9))*0.5;*/
	float xx = x / (40.0/width);
	float yy = y / (40.0/height);

	float dx = fmod(xx,1);
	float dy = fmod(yy,1);

	float h1 = noise_map[(int)xx][(int)yy];
	float h2 = noise_map[(int)xx+1][(int)yy];
	float h3 = noise_map[(int)xx][(int)yy+1];
	float h4 = noise_map[(int)xx+1][(int)yy+1];

	if (dx > dy)
		return (h1*(1-dx)+h2*dx)*(1-dy)+(h3*(1-dx)+h4*dx)*dy-0.5;
	else
		return (h1*(1-dy)+h3*dy)*(1-dx)+(h2*(1-dy)+h4*dy)*dx-0.5;
}
void Projection::test(){
	float xSpacing = 4.0/width;
	float ySpacing = 4.0/height;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.0,0.0,0.0,0.0);
	glPointSize(5);
	glColor3f(0,0,1);


	glPushMatrix();
	glTranslatef(-1,0.001,0);
	glBegin(GL_QUADS);
	for(int x = 1; x < width; x++){
		for(int y = 1; y < height; y++){
			float k = 1.8;
			float trans = 1.0;
			float color = noise_map3D[x][y][(int)time]*k;
			glColor4f(1-color,1-color,1-color,color*trans);
			glVertex3f(x*xSpacing,noise_map[x][y]/5.0,y*ySpacing);


			color = noise_map3D[x-1][y][(int)time]*k;
			glColor4f(1-color,1-color,1-color,color*trans);
			glVertex3f((x-1)*xSpacing,noise_map[x-1][y]/5.0,y*ySpacing);

			color = noise_map3D[x-1][y-1][(int)time]*k;
			glColor4f(1-color,1-color,1-color,color*trans);
			glVertex3f((x-1)*xSpacing,noise_map[x-1][y-1]/5.0,(y-1)*ySpacing);

			color = noise_map3D[x][y-1][(int)time]*k;
			glColor4f(1-color,1-color,1-color,color*trans);
			glVertex3f(x*xSpacing,noise_map[x][y-1]/5.0,(y-1)*ySpacing);

		}
	}
	time += dt;
	if (time > noise_map3D[0][0].size()-1 || time < 0) {
		dt*=-1;
		time += dt;
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
}

void Projection::renderLandscape(){
	glRotatef(90,1,0,0);

	float tx = 10.0/width;
	float ty = 10.0/height;



	float xSpacing = 40.0/width;
	float ySpacing = 40.0/height;
	float zConst = 0.9;

	glPushMatrix();
	glTranslatef(-1,0,0);
	glBegin(GL_QUADS);
	glColor3f(1,0,0);
	float transp = 1;
	for (int x = 1; x < width; x++){
		for (int y = 1; y < height; y++){
			float color =  noise_map[x-1][y-1]*0.5;

			//cout << (y-1)*ty << " " << y*ty << endl;
			glColor4f(color,color,color,transp);

			glTexCoord2f((x-1)*tx,(y-1)*ty);
			glVertex3f((x-1)*xSpacing,noise_map[x-1][y-1]*zConst,(y-1)*ySpacing);


			color = noise_map[x-1][y]*0.5;
			glColor4f(color,color,color,transp);

			glTexCoord2f((x-1)*tx,y*ty);
			glVertex3f((x-1)*xSpacing,noise_map[x-1][y]*zConst,y*ySpacing);


			color = noise_map[x][y]*0.5;
			glColor4f(color,color,color,transp);
			glTexCoord2f(x*tx,y*ty);
			glVertex3f(x*xSpacing,noise_map[x][y]*zConst,y*ySpacing);



			color = noise_map[x][y-1]*0.5;
			glColor4f(color,color,color,transp);
			glTexCoord2f(x*tx,(y-1)*ty);
			glVertex3f(x*xSpacing,noise_map[x][y-1]*zConst,(y-1)*ySpacing);
		}
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

}

void Projection::testTexture(){
	float xSpacing = 1.0/width;
	float ySpacing = 1.0/height;

	glPointSize(5);
	glBegin(GL_POINTS);
	for (int x = 1; x < width; x++){
		for (int y = 1; y < height; y++){
			float genValue = noise_map[x][y];
			float r = (((244-108)*genValue)+108)/255;
			float g = (((201-84)*genValue)+84)/255;
			float b = (((175-30)*genValue)+30)/255;

			glColor3f(r,g,b);
			glVertex3f(x*xSpacing,y*ySpacing,0);
		}
	}
	glEnd();
}

//
//Interpolation simply works between 2 values, and as such is dimentionally independent
//
float Projection::interpolation(float v1,float v2, float dx){
	float ft = dx * 3.1415927;
	float f = (1-cos(ft)) * 0.5;
	return v1*(1-f) + v2*f;
}

//
//Better interpolation
//
//v1 = p-2 v2 = p-1 v3 = p-1 v4 = p-2
//dx = p - v2
float Projection::cubicInterpolation(float v1,float v2,float v3,float v4, float dx){
	float a = (v4 - v3)-(v1-v2);
	float b = (v1-v2)-a;
	float c = v3-v1;
	float d = v1;

	return (a*pow(dx,3))+(b*pow(dx,2))+(c*dx)+d;
}

//
//1 DIMENTIONAL PERLIN CALLS
//
float Projection::noise(int x){
	x+=seed;
	x = (x<<13)^x;
	return ( 1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float Projection::smoothNoise(float dx){
	int p1 = dx - fmod(dx,1);

	float v1 = interpolation(noise(p1-1),noise(p1),fmod(dx,1));
	float v2 = interpolation(noise(p1),noise(p1+1),fmod(dx,1));
	float v3 = interpolation(noise(p1+1),noise(p1+2),fmod(dx,1));

	return (v1/4)+(v2/2)+(v3/4);
}

float Projection::perlinNoise(float x){
	float total = 0;

	int octaves = 10;
	float persistence = 0.1;

	for (int i = 0; i < octaves; i++){
		float frequency = pow(2,i);
		float ampitude = pow(persistence,i);

		total += smoothNoise(x*frequency)*ampitude;
	}


	return total;
}

//
//2 DIMENTIONAL PERLIN CALLS
//

float Projection::noise2D(int x,int y){
	int n = x+y*50;
	n = (n<<13)^n;
	n+=seed;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float Projection::smoothNoise2D(float dx,float dy){
	float corners = (noise2D(dx-1,dy-1) + noise2D(dx+1,dy-1) + noise2D(dx-1,dy+1)+noise2D(dx+1,dy+1))/16;
	float sides = (noise2D(dx-1, dy)  +noise2D(dx+1, dy)  +noise2D(dx, dy-1)  +noise2D(dx, dy+1) ) /8;
	float center  =  noise2D(dx, dy) / 4;
	return corners+sides+center;
}

float Projection::interpolateNoise2D(float x, float y){
	float fx = fmod(x,1);
	int ix = x-fx;

	float fy = fmod(y,1);
	int iy = y-fy;



	float v1 = smoothNoise2D(ix,iy);
	float v2 = smoothNoise2D(ix+1,iy);
	float v3 = smoothNoise2D(ix,iy+1);
	float v4 = smoothNoise2D(ix+1,iy+1);

	float i1 = interpolation(v1,v2,fx);
	float i2 = interpolation(v3,v4,fx);

	return interpolation(i1,i2,fy);
}

float Projection::perlinNoise2D(float x, float y){
	float total = 0;

	int octaves = 4;
	float persistence = 0.25;

	for (int i = 0; i < octaves; i++){
		float frequency = pow(2,i);
		float ampitude = pow(persistence,i);

		total += interpolateNoise2D(x*frequency,y*frequency)*ampitude;
	}
	total++;
	return total;
}

//
//3 DIMENTIONAL PERLIN CALLS
//

float Projection::noise3D(int x,int y,int z){
	int n = x+y*1000+z*100000;
	n = (n<<13)^n;
	n+=seed;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float Projection::smoothNoise3D(float dx,float dy,float dz){
	float sides = (noise3D(dx,dy,dz-1),noise3D(dx,dy,dz+1))/16;
	float corners = (noise3D(dx-1,dy-1,dz) + noise3D(dx+1,dy-1,dz) + noise3D(dx-1,dy+1,dz)+noise3D(dx+1,dy+1,dz))/32;
	float edges = (noise3D(dx-1,dy,dz),noise3D(dx-1,dy,dz),noise3D(dx,dy+1,dz),noise3D(dx,dy+1,dz))/16;
	float center = noise3D(dx,dy,dz)/4;
	return center+edges+corners+sides;
}

float Projection::interpolateNoise3D(float x, float y, float z){
	float fx = fmod(x,1);
	int ix = x;

	float fy = fmod(y,1);
	int iy = y;

	float fz = fmod(z,1);
	int iz = z;


	float v1 = smoothNoise3D(ix,iy,iz);
	float v2 = smoothNoise3D(ix+1,iy,iz);
	float v3 = smoothNoise3D(ix,iy+1,iz);
	float v4 = smoothNoise3D(ix+1,iy+1,iz);

	float v5 = smoothNoise3D(ix,iy,iz+1);
	float v6 = smoothNoise3D(ix+1,iy,iz+1);
	float v7 = smoothNoise3D(ix,iy+1,iz+1);
	float v8 = smoothNoise3D(ix+1,iy+1,iz+1);


	float i1 = interpolation(v1,v2,fx);
	float i2 = interpolation(v3,v3,fx);

	float i3 = interpolation(v5,v6,fx);
	float i4 = interpolation(v7,v8,fx);

	float out1 = interpolation(i1,i2,fy);
	float out2 = interpolation(i3,i4,fy);

	return interpolation(out1,out2,fz);
}

float Projection::perlinNoise3D(float x, float y, float z){
	float total = 0;

	int octaves = 4;
	float persistence = 0.25;

	for (int i = 0; i < octaves; i++){
		float frequency = pow(2,i);
		float ampitude = pow(persistence,i);

		total += interpolateNoise3D(x*frequency,y*frequency,z*frequency)*ampitude;
	}

	total+=0.3;
	if (total < 0) total = 0;
	//
	//ADJUST CONTRAST FOR CAUSTIC EFFECT
	//
	//
	//	if (total < 0.3) total =0;
	//	if (total < 0.45) total *=0.5;

	if (total >0.3 &&total <0.4 ) total = 1;
	else total = 0;
	//
	//AWWWWWW YISSS
	//
	return total;
}



