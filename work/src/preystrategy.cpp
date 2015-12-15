#include <vector>
#include "preystrategy.hpp"
#include "boid.hpp"
#include "scenemanager.hpp"
#include "mainheaders.hpp"
#include <math.h>

using namespace glm;
using namespace std;

/*
* Move toward the centre
*/
vec3 PreyStrategy::keepInScreen(Boid* parent, SceneManager* scene){
	// Centered
	vec3 centered = vec3(0, 0, 0);
	vec3 size = (scene->getXYZMax() - scene->getXYZMin()) * 0.5f;
	vec3 center = scene->getXYZMin() + size;
	//size *= 0.5f;
	centered = center - parent->getPosition();
	centered /= (scene->getXYZMax() - scene->getXYZMin()) * 0.5f;
	if (length(centered) < 0.5f)
		return vec3();
	/**
	vec3 p = parent->getPosition();
	if (p.x < center.x - size.x) centered.x = 1;
	if (p.x > center.x + size.x) centered.x = -1;

	if (p.y < center.y - size.y) centered.y = 1;
	if (p.y > center.y + size.y) centered.y = -1;

	if (p.z < center.z - size.z) centered.z = 1;
	if (p.z > center.z + size.z) centered.z = -1;*/

	return centered;
}

/*
* Keep above the sand and below the surface
*/
vec3 PreyStrategy::keepInSea(Boid* parent, SceneManager* scene){
	vec3 steerAmount;
	vec3 pos = parent->getPosition();
	vec3 dir = parent->getDirection();

	float floorY = scene->getTerrainHeight(pos.x, pos.z);
	if (pos.y - floorY < 10){
		steerAmount.x = dir.x;
		steerAmount.y = 1;
		steerAmount.z = dir.z;
	}

	return steerAmount;
}

/**
* Applies main rules
*/
vec3 PreyStrategy::mainRules(vector<Boid*>* b, Boid* parent, bool *pred){
	vec3 separation = vec3(0, 0, 0);
	vec3 alignment = vec3(0, 0, 0);
	vec3 cohesion = vec3(0, 0, 0);
	vec3 predator = vec3(0, 0, 0);

	int sepCount = 0;
	int cohCount = 0;

	for (vector<Boid*>::iterator i = b->begin(); i != b->end(); ++i){
		if (&(*(*i)) != &(*parent) && dot(parent->getDirection(), (*i)->getPosition() - parent->getPosition()) > 0){
			alignment += (*i)->getDirection();
			cohesion += (*i)->getPosition();
			cohCount++;
		}

		if (&(*(*i)) != &(*parent)){
			if (length(parent->getPosition() - (*i)->getPosition()) < 40.f){

				separation += (parent->getPosition() - (*i)->getPosition());
				sepCount++;
			}
			if ((*i)->getFishType() == PREDATOR){
				predator += (*i)->getPosition() + (*i)->getDirection() * (*i)->getSize();
				*pred = true;
			}
		}
	}

	if (cohCount > 0){
		// Cohesion
		cohesion /= cohCount;
		cohesion = (cohesion - parent->getPosition()) - parent->getDirection();

		// Alignment
		alignment /= cohCount;
		alignment -= parent->getDirection();
	}

	// Seperation
	if (sepCount > 0){
		separation /= sepCount;
		separation -= parent->getDirection();
		separation = normalize(separation);
	}


	// Swim from Predators
	if (*pred){
		predator /= (b->size() - 1.f);
		predator = normalize(parent->getPosition() + parent->getDirection() * parent->getSize() - predator);
	}

	//return cohesion;// *0.5f + predator; alignment + 
	return separation + alignment * 0.2f + cohesion * 0.1f + predator;
}


/*
* Returns a vec3 of where to go next
*/
vec3 PreyStrategy::applyRules(Boid* parent, SceneManager* scene){
	vec3 halfRegion = vec3(40.f);
	vector<Boid*>* b = scene->getBoids()->searchRegion(parent->getPosition() - halfRegion, parent->getPosition() + halfRegion);
	vec3 dir = parent->getDirection() + keepInScreen(parent, scene) +keepInSea(parent, scene);
	bool pred = false;

	if (b->size() > 1){
		dir += mainRules(b, parent, &pred);
	}

	delete b;
	if (pred)
		return normalize(dir) * 2.f;
	return normalize(dir);
}
