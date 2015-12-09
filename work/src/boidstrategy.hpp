#ifndef BOIDSTRATEGY_H
#define BOIDSTRATEGY_H
#include "mainheaders.hpp"

class SceneManager;
class Boid;
/*
*	This class defines a strategy pattern for the boid to follow
*/

class BoidStrategy {
public:
	virtual glm::vec3 applyRules(Boid* p, SceneManager* s) = 0;
};
#endif 