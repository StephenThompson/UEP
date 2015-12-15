#include "mainheaders.hpp"

#include "boid.hpp"
#include "boidsystem.hpp"
#include "scenemanager.hpp"
#include <math.h>

using namespace std;
using namespace glm;

BoidSystem::BoidSystem(int count, SceneManager* scene, Geometry* model, BoidStrategy* strategy, float scale, FishType type){
	this->model = model;
	this->scene = scene;

	vec3 dimension = (scene->getXYZMax() - scene->getXYZMin());
	vec3 groupPos;
	vec3 groupDir;
	int nextGroup = 0;

	for (unsigned int i = 0; i < count; ++i){
		// Sets up a new group of boids
		if (i == nextGroup){
			groupPos = vec3(scene->getXYZMin().x + dimension.x * 0.15f + ((rand() % 200) / 200.f)*(dimension.x*0.7f),
							scene->getXYZMin().y + dimension.y * 0.15f + ((rand() % 200) / 200.f)*(dimension.y*0.7f),
							scene->getXYZMin().z + dimension.z * 0.15f + ((rand() % 200) / 200.f)*(dimension.z*0.7f));

			groupDir = vec3(((rand() % 200) / 100.f - 1), ((rand() % 200) / 100.f - 1), ((rand() % 200) / 100.f - 1));
			nextGroup += rand() % (count - i) * 0.5 + 4;
		}

		// Creates Boid
		vec3 pos = groupPos + vec3(((rand() % 200) / 200.f) * dimension.x * 0.1f, ((rand() % 200) / 200.f) * dimension.y * 0.1f, ((rand() % 200) / 200.f)* dimension.z * 0.1f);
		vec3 dir = groupDir + vec3(((rand() % 200) / 100.f - 1), ((rand() % 200) / 100.f - 1), ((rand() % 200) / 100.f - 1)) * 0.1f;

		Boid* b = new Boid(scene, strategy, pos, dir, scale, type);
		boids.push_back(b);
		scene->getBoids()->insert(b);
	}
}

BoidSystem::~BoidSystem(){
	for (unsigned int i = 0; i< boids.size(); ++i){
		delete boids[i];
	}
	this->model = nullptr;
	this->scene = nullptr;
}

/*
* Moves the boid simulation forward by one step
*/
void BoidSystem::stepSimulation(){
	for (vector<Boid*>::iterator i = boids.begin(); i != boids.end(); ++i){
		(*i)->stepSimulation();
		this->scene->getBoids()->update(*i);
	}
}

/*
* Draws the boids in this system
*/
void BoidSystem::draw(){
	for (vector<Boid*>::iterator i = boids.begin(); i != boids.end(); ++i){
		glPushMatrix();
		glMultMatrixf(&((*i)->getMatrix())[0][0]);
		model->renderGeometry((*i)->getTime());
		glPopMatrix();
	}
}
