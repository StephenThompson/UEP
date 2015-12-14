#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "mainheaders.hpp"
#include "projection.hpp"


using namespace std;

Projection::Projection() {
	cout << "Generating landscape..." << endl;
	width = 200+trim;
	height = 200+trim;
	scale = 500;
	bumpiness = 1.f;
	for (int x = 0; x < width; x++) {
		vector<float> temp;
		for (int y = 0; y < height; y++) {
			temp.push_back(random()*scale);
		}
		heights.push_back(temp);
	}
	addHills();
	
	for (int i = 0; i < 2; i++) {
		smoothPoints();
	}
	cout << "Done." << endl;
}

void Projection::setWeights(int width) {
	int size = width;

	for (int x = 0; x < size; x++) {
		vector<float> temp = vector<float>();
		for (int y = 0; y < size; y++) {
			temp.push_back(0);
		}
		weights.push_back(temp);
	}

	int ranks = size / 2 + 1;

	for (int i = 0; i < ranks; i++) {
		for (int y = 0; y < ranks; y++) {
			for (int x = 0; x < ranks; x++) {
				if (x >= i || y >= i) {
					int fract = 8 * i;
					if (fract == 0) fract++;
					fract *= ranks;
					weights[ranks - x - 1][ranks - y - 1] = 1.f / fract;
					weights[ranks + x - 1][ranks - y - 1] = 1.f / fract;
					weights[ranks + x - 1][ranks + y - 1] = 1.f / fract;
					weights[ranks - x - 1][ranks + y - 1] = 1.f / fract;
				}
			}
		}
	}
}

void Projection::addHills() {
	int base = 5;
	
	int z = (int)(random() * scale + scale/2.f);
	
	for (int x = base; x < width - base; x++) {
		for (int y = base; y < height - base; y++) {
			
			if (random() < 0.015) {
				for (int hx = 0; hx < base; hx++) {
					for (int hy = 0; hy < base; hy++) {
						heights[x + hx][y + hy] = -random()*(z - hx);
						heights[x + hx][y - hy] = -random()*(z - hy);
						heights[x - hx][y - hy] = -random()*(z - hx);
						heights[x - hx][y + hy] = -random()*(z - hy);
						if (random() < 0.5) break;	
					}
				}
			}
		}
	}
}
float Projection::random() {
	return (rand() / (RAND_MAX + 0.f));
}

void Projection::smoothPoints() {
	int range = 10;
	setWeights((range * 2) + 1);
	vector<vector<float>> temp;
	for (int x = 0; x < width; x++) {
		vector<float> empty;
		for (int y = 0; y < height; y++) {
			empty.push_back(0);
		}
		temp.push_back(empty);
	}

	for (int x = range; x < width; x++) {
		for (int y = range; y < height; y++) {
			float value = 0;
			if (x > range && x < width - range && y > range && y < height - range) {
				for (int ox = 0; ox < (range * 2) + 1; ox++) {
					for (int oy = 0; oy < (range * 2) + 1; oy++) {
						value += heights[x + ox - range][y + oy - range] * weights[ox][oy];
					}
				}
				temp[x][y] = value;
			}
		}
	}
	heights = temp;
}

void Projection::renderLandscape(){

	float tx = 10.0/width;
	float ty = 10.0/height;



	float xSpacing = 4000.f/width;
	float ySpacing = 4000.f/height;

	
	if (m_displaylist == 0) {
		glNewList(m_displaylist, GL_COMPILE);

		glBegin(GL_QUADS);
		glColor3f(1, 0, 0);
		float transp = 1;
		float drop = 550;
		for (int x = trim; x < width- trim; x++) {
			for (int y = trim; y < height - trim; y++) {
				glTexCoord2f((x - 1)*tx, (y - 1)*ty);
				glVertex3f(((x - width / 2.f) - 1)*xSpacing, heights[x - 1][y - 1]-drop, ((y - height / 2.f) - 1)*ySpacing);

				glTexCoord2f((x)*tx, (y - 1)*ty);
				glVertex3f(((x - width / 2.f))*xSpacing, heights[x][y - 1] - drop, ((y - height / 2.f) - 1)*ySpacing);

				glTexCoord2f((x)*tx, (y)*ty);
				glVertex3f(((x - width / 2.f))*xSpacing, heights[x][y] - drop, ((y - height / 2.f))*ySpacing);

				glTexCoord2f((x - 1)*tx, (y)*ty);
				glVertex3f(((x - width / 2.f) - 1)*xSpacing, heights[x - 1][y] - drop, ((y - height / 2.f)*ySpacing));
			}
		}
		glEnd();
		glEndList();
	}
	glCallList(m_displaylist);
	
	glDisable(GL_BLEND);

}

float Projection::getHeight(float x, float y) {
	int x1 = (int)(x/(4000.f / width) + (width / 2.f));
	int y1 = (int)(y/(4000.f / height) + (height / 2.f));
	return heights[x1][y1] - 550;
}




