#include <iostream>
#include <string>
#include <vector>

#include "mainheaders.hpp"

using namespace glm;

class Branch{
	public:
	Branch *Parent;
	vec3 GrowDirection;
	vec3 OriginalGrowDirection;
	float GrowCount = 0;
	vec3 Position;
	float base;
	float top;
	float minWidth = 0.03f;

	Branch(Branch*, vec3, vec3, float, float);
	void Reset();

};
