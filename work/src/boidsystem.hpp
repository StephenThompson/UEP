#include <vector>
#include "geometry.hpp"

class SceneManager;
class BoidStrategy;
class Boid;

class BoidSystem {
private:
	Geometry* model; //To add
	std::vector<Boid*> boids;
	SceneManager* scene;
public:
	BoidSystem(int, SceneManager*, Geometry*, BoidStrategy*, float, FishType type);
	~BoidSystem();
	void stepSimulation();
	void draw();
};
