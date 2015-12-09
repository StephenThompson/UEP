#include <vector>
#include "scenemanager.hpp"
#include "boidsystem.hpp"
#include "mainheaders.hpp"

using namespace std;
using namespace glm;

SceneManager::SceneManager(vec3 xyzMin, vec3 xyzMax){
	this->xyzMin = xyzMin;
	this->xyzMax = xyzMax;
	boidTree = new Octree(xyzMin, xyzMax);

}

SceneManager::~SceneManager(){
	for (unsigned int i = 0; i < systems.size(); ++i){
		delete systems[i];
	}
	delete boidTree;
}
/*
* Adds a new boid system to the scene
*/
void SceneManager::addSystem(int count, Geometry* model, BoidStrategy* strategy, float scale, FishType type){
	systems.push_back(new BoidSystem(count, this, model, strategy, scale, type));
}

/*
* Returns a tree of boids in this scene
*/
Octree* SceneManager::getBoids(){
	return boidTree;
}

/*
* Returns xyzMin of this scene
*/
vec3 SceneManager::getXYZMin(){
	return xyzMin;
}

/*
* Returns xyzMax of this scene
*/
vec3 SceneManager::getXYZMax(){
	return xyzMax;
}

/*
* Steps the simulation forward by one step
*/
void SceneManager::stepSimulation(){
	for (vector<BoidSystem*>::iterator i = systems.begin(); i != systems.end(); ++i){
		(*i)->stepSimulation();
	}
}

/*
* Draw all of the boid systems in this scene
*/
void SceneManager::draw(){
	if (debugMode){
		glDisable(GL_TEXTURE_2D);
		glUseProgram(0);
		boidTree->debugDraw();
	}
	for (vector<BoidSystem*>::iterator i = systems.begin(); i != systems.end(); ++i){
		(*i)->draw();
	}
}

/*
* Toggles the octree debug mode
*/
void SceneManager::toggleDebug(){
	debugMode = !debugMode;
}
