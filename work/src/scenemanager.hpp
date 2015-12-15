#include <vector>

//#include <glm\glm.hpp>
#include "geometry.hpp"
#include "octree.hpp"

class BoidSystem;
class BoidStrategy;
class Boid;
class Projection;

/*
* Manages the boid systems in the scene
*/
class SceneManager {
private:
	std::vector<BoidSystem*> systems;
	std::vector<Boid*> boids;
	Octree* boidTree;
	Projection* proj;
	glm::vec3 xyzMin;
	glm::vec3 xyzMax;
	bool debugMode = false;

public:
	SceneManager(glm::vec3 xyzMin, glm::vec3 xyzMax, Projection* proj);
	~SceneManager();
	void addSystem(int, Geometry*, BoidStrategy*, float, FishType);

	Octree* getBoids();
	void stepSimulation();
	void toggleDebug();
	float getTerrainHeight(float, float);
	glm::vec3 getXYZMin();
	glm::vec3 getXYZMax();
	void draw();
};
