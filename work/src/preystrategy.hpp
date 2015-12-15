#ifndef PREYSTRATEGY_H
#define PREYSTRATEGY_H
#include "mainheaders.hpp"
#include "boidstrategy.hpp"
#include <vector>

class SceneManager;
class Boid;

/*
*	This class defines a strategy pattern for the boid to follow
*/

class PreyStrategy : public BoidStrategy {
	private:
		glm::vec3 keepInScreen(Boid* parent, SceneManager* scene); 
		glm::vec3 keepInSea(Boid* parent, SceneManager* scene);
		glm::vec3 mainRules(std::vector<Boid*>* b, Boid* parent, bool*);
	public:
		glm::vec3 applyRules(Boid* b, SceneManager* s);
};
#endif
