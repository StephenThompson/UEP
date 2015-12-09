/*
 * branch.cpp
 *
 *  Created on: 15/10/2015
 *      Author: gillcall1
 */

#include "mainheaders.hpp"
#include "branch.hpp"
using namespace std;
using namespace glm;


Branch::Branch(Branch *parent, vec3 position, vec3 growDirection, float baseWidth, float topWidth)
{
	Parent = parent;
	Position = position;
	GrowDirection = growDirection;
	OriginalGrowDirection = growDirection;
	if(baseWidth<minWidth){
	  base=minWidth;
	}else{
	  base = baseWidth;
	}
	if(topWidth<minWidth){
	  top = minWidth;
	}else{
	  top = topWidth;
	}
}

void Branch::Reset()
{
	GrowCount = 0;
	GrowDirection = OriginalGrowDirection;
}
