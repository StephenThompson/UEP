#include "mainheaders.hpp"

#include "boid.hpp"
#include "boidsystem.hpp"
#include "scenemanager.hpp"

using namespace std;
using namespace glm;

BoidSystem::BoidSystem(int count, SceneManager* scene, Geometry* model, BoidStrategy* strategy, float scale, FishType type){
	this->model = model;
	this->scene = scene;

	for (unsigned int i = 0; i < count; ++i){
		Boid* b = new Boid(scene, strategy, scale, type);
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
