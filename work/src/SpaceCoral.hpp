/*
 * SpaceCoral.hpp
 *
 *  Created on: 15/10/2015
 *      Author: deightcull
 */

#include <vector>
#include <cmath>
#include <map>

#include "branch.hpp"
#include "shape.hpp"
#include "mainheaders.hpp"

using namespace glm;
using namespace std;

struct GrowPoint{
	Branch *nearestBranch;
	glm::vec3 position;
};

class SpaceCoral{
private:
	vec3 m_position;

	bool growing = true;
	GLuint m_displayList = 0;

	vec3 color = vec3(0.5f, 0.5f, 0.5f);
//	vec3 m_transPos;
	float scaleCoral = 1.0f;
	int growPointCount = 500;
	int maxBranches = 1000;
	float coralWidth = 80;
	float coralHeight = 150;
	float baseHeight = 1*scaleCoral;
	float minDistance = 0.3*scaleCoral;
	float maxDistance = 2*scaleCoral;
	float branchLength = 0.25f*scaleCoral;
	float branchShrink = 0.9f;
	float initialWidth = 0.5*scaleCoral;

	Branch root = Branch(nullptr, vec3(0,0,0), vec3(0,0,0), 0, 0);
	vector<Branch*> m_branches;

	vector<GrowPoint> growthPoints;
	vector<GrowPoint> growthPointsOrignal;

	Shape m_crown = Shape(vec3(0,0,0), 0, false);

	void GenerateBase();
	void GenerateGrowthPoints();
	void DrawBranch(vec3 pos, vec3 dir, float, float, float);
	void createDisplayList();

public:
	SpaceCoral(vec3 pos, Shape s, float, float, vec3);
	void Grow();
	void Draw();

};
