//#include "boidstrategy.hpp"
#include "mainheaders.hpp"

class SceneManager;
class BoidStrategy;

class Boid {
private:
	BoidStrategy* strategy;
	SceneManager* sceneMng;
	glm::vec3 position;
	glm::vec3 oldPosition;
	glm::vec3 velocity;
	glm::vec3 direction;
	float scaleSize;
	float time;
	FishType type;

public:
	Boid(SceneManager*, BoidStrategy*, glm::vec3, glm::vec3, float, FishType type);
	~Boid();
	void stepSimulation();
	glm::vec3 getPosition();
	glm::vec3 getOldPosition();
	glm::vec3 getDirection();
	glm::mat4 getMatrix();
	float getTime();
	float getSize();
	FishType getFishType();
};
