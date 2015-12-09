#include <vector>
#include "predatorstrategy.hpp"
#include "boid.hpp"
#include "scenemanager.hpp"
#include "mainheaders.hpp"

using namespace glm;
using namespace std;

/*
* Move toward the centre
*/
vec3 PredatorStrategy::keepInScreen(Boid* parent, SceneManager* scene){
	// Centered
	vec3 centered = vec3(0, 0, 0);
	vec3 center = scene->getXYZMin() + (scene->getXYZMax() - scene->getXYZMin()) * 0.5f;

	centered = center - parent->getPosition();
	centered /= length(scene->getXYZMax() - scene->getXYZMin());
	if (length(centered) < 0.5f)
		return vec3();
	return centered;
}

/**
* Applies main rules
*/
vec3 PredatorStrategy::mainRules(vector<Boid*>* b, Boid* parent){
	vec3 cohesion = vec3(0, 0, 0);
	bool applyCohesion = false;

	for (vector<Boid*>::iterator i = b->begin(); i != b->end(); ++i){
		if (&(*(*i)) != &(*parent) && dot(parent->getDirection(), (*i)->getPosition() - parent->getPosition()) > 0){// 0 && (*i)->getFishType() == PREY){
			cohesion += (*i)->getPosition();
			applyCohesion = true;
		}
	}

	// Cohesion
	if (applyCohesion){
		cohesion /= (b->size() - 1.f);
		cohesion = normalize(cohesion - parent->getPosition());
	}
	return cohesion;
}

/*
* Returns a vec3 of where to go next
*/
vec3 PredatorStrategy::applyRules(Boid* parent, SceneManager* scene){
	vec3 halfRegion = vec3(20.f);
	//vector<Boid*>* b = scene->getBoids()->searchRegion(parent->getPosition() - halfRegion, parent->getPosition() + halfRegion);
	vec3 dir = parent->getDirection() + keepInScreen(parent, scene);
	/*
	if (b->size() > 2){
		dir += mainRules(b, parent);
	}

	delete b;*/
	return normalize(dir)*4.f;
}
