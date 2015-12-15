#include "boid.hpp"
#include "boidstrategy.hpp"
#include "mainheaders.hpp"
#include "scenemanager.hpp"
#include <iostream>

using namespace std;
using namespace glm;

Boid::Boid(SceneManager* scene, BoidStrategy* strategy, vec3 position, vec3 direction, float scale, FishType type){
	this->strategy = strategy;
	this->sceneMng = scene;
	this->type = type;
	this->position = position;
	this->direction = direction;

	oldPosition = position + direction;
	scaleSize = scale * (rand() % 100 / 100.f + 2.f);
	time = radians(float(rand() % 360));
}

Boid::~Boid(){
}

/*
* Moves the boid forward by one step
*/
void Boid::stepSimulation(){
	vec3 newPos = strategy->applyRules(this, sceneMng);
	direction = newPos;
	oldPosition = position;
	position += direction;

	time += 0.2f;
	if (time >= radians(360.f))
		time = 0;
}

/*
* Returns the position of this boid.
*/
vec3 Boid::getPosition(){
	return position;
}

/*
* Returns the position of this boid
*/
vec3 Boid::getOldPosition(){
	return oldPosition;
}


/*
* Returns the direction of this boid
*/
vec3 Boid::getDirection(){
	return direction;
}

/*
 * Returns the size of this boid
 */
float Boid::getSize(){
	return scaleSize;
}

/*
* Returns the direction of this boid
*/
mat4 Boid::getMatrix(){
	vec3 d = normalize(position - oldPosition);
	float pitch = (float)(asin(-d.y));
	float yaw = (float)(atan2(d.z, -d.x));

	mat4 ret = mat4(1);
	ret = translate(ret, position);
	ret = rotate(ret, yaw, vec3(0, 1, 0));
	ret = rotate(ret, pitch, vec3(0, 0, 1));

	ret = scale(ret, vec3(scaleSize, scaleSize, scaleSize));

	return ret;
}

/*
* Returns time in radians
*/
float Boid::getTime(){
	return time;
}

/*
 * Returns the type of fish - Prey or Predator
 */
FishType Boid::getFishType(){
	return type;
}
