#include <vector>
#include "preystrategy.hpp"
#include "boid.hpp"
#include "scenemanager.hpp"
#include "mainheaders.hpp"

using namespace glm;
using namespace std;

/*
* Move toward the centre
*/
vec3 PreyStrategy::keepInScreen(Boid* parent, SceneManager* scene){
	// Centered
	vec3 centered = vec3(0, 0, 0);
	vec3 center = scene->getXYZMin() + (scene->getXYZMax() - scene->getXYZMin()) * 0.5f;

	centered = center - parent->getPosition();
	centered /= length((scene->getXYZMax() - scene->getXYZMin())*0.25f);
	if (length(centered) < 0.5f)
		return vec3();
	return centered;
}

/**
* Applies main rules
*/
vec3 PreyStrategy::mainRules(vector<Boid*>* b, Boid* parent, bool *pred){
	vec3 separation = vec3(0, 0, 0);
	vec3 alignment = vec3(0, 0, 0);
	vec3 cohesion = vec3(0, 0, 0);
	vec3 predator = vec3(0, 0, 0);
	bool sep = false;

	for (vector<Boid*>::iterator i = b->begin(); i != b->end(); ++i){
		if (&(*(*i)) != &(*parent) && dot(parent->getDirection(), (*i)->getPosition() - parent->getPosition()) > 0){
			alignment += (*i)->getDirection();
			cohesion += (*i)->getPosition();
		}

		if (&(*(*i)) != &(*parent)){
			if (length((*i)->getPosition() - parent->getPosition()) < (parent->getSize() + (*i)->getSize()) * 2.f){
				separation += (*i)->getPosition();
				sep = true;
			}
			if ((*i)->getFishType() == PREDATOR){
				predator += (*i)->getPosition() + (*i)->getDirection() * (*i)->getSize();
				*pred = true;
			}
		}

	}


	// Cohesion
	cohesion /= (b->size() - 1.f);
	cohesion = normalize(cohesion - parent->getPosition());

	// Seperation
	if (sep){
		separation /= (b->size() - 1.f);
		separation = normalize(parent->getPosition() - separation);
	}

	// Alignment
	alignment /= (b->size() - 1);

	// Swim from Predators
	if (*pred){
		predator /= (b->size() - 1.f);
		predator = normalize(parent->getPosition()  + parent->getDirection() * parent->getSize() - predator);
	}

	return separation + alignment * 0.7f + cohesion * 0.2f + predator;
}

/*
* Returns a vec3 of where to go next
*/
vec3 PreyStrategy::applyRules(Boid* parent, SceneManager* scene){
	vec3 halfRegion = vec3(40.f);
	vector<Boid*>* b = scene->getBoids()->searchRegion(parent->getPosition() - halfRegion, parent->getPosition() + halfRegion);
	vec3 dir = parent->getDirection() + keepInScreen(parent, scene);
	bool pred = false;

	if (b->size() > 2){
		dir += mainRules(b, parent, &pred);
	}

	delete b;
	if (pred)
		return normalize(dir) * 2.f;
	return normalize(dir);
}
