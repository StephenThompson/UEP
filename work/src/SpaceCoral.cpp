/*
 * SpaceCoral.cpp
 *
 *  Created on: 15/10/2015
 *      Author: gillcall1
 */
#include "SpaceCoral.hpp"

using namespace glm;
using namespace std;

SpaceCoral::SpaceCoral(vec3 position, Shape s, float baseSize, float baseWidth, vec3 col){
//	m_transPos = position;
	m_position = position;
	baseHeight = baseSize*scaleCoral;
	initialWidth = baseWidth*scaleCoral;
	color=col;
	m_crown = s;//Shape(position + vec3(0, baseHeight+3, 0), 3.0f, true);//vec3(2, 5, 2)
	GenerateBase();
	GenerateGrowthPoints();
}

void SpaceCoral::GenerateGrowthPoints(){
	growthPoints.clear();
	for (int i = 0; i < growPointCount; i++)
	{
		vec3 location = m_crown.randPoint();
		GrowPoint point;
		point.nearestBranch=nullptr;
		point.position=location;
		growthPoints.push_back(point);
	}
	growthPointsOrignal=growthPoints;
}

void SpaceCoral::GenerateBase(){
	m_branches.clear();
	root = Branch(nullptr, m_position, vec3(0, 1, 0), initialWidth, initialWidth);
	m_branches.push_back(&root);

	//new Branch(Root, new Vector2(Position.X, Position.Y - BranchLength), new Vector2(0, -1));
	Branch *current = new Branch(&root, vec3(m_position.x, m_position.y + branchLength, m_position.z), vec3(0,1,0), initialWidth, initialWidth*branchShrink);
	m_branches.push_back(current);

	while(length(root.Position - current->Position) < baseHeight){
		Branch *next = new Branch(current, vec3(current->Position.x, current->Position.y+branchLength, current->Position.z), vec3(0,1,0), current->top, current->top*branchShrink);
		m_branches.push_back(next);
		current = next;
	}
}

void SpaceCoral::Grow(){
	while(growing){

		//If no points left
		if(growthPoints.size()<=0){
			growing=false;
			return;
		}

		//Process growth points
		for (int i = 0; i < growthPoints.size(); i++){
			bool pointRemoved = false;

			growthPoints.at(i).nearestBranch = nullptr;
			vec3 direction;

			//Find the nearest branch for this point
			for(Branch *b : m_branches){
				direction = growthPoints.at(i).position - b->Position;               //direction to branch from leaf
				float distance = length(direction);           //distance to branch from leaf
				direction = normalize(direction);

				//minimum distance to point reached, we remove it
				if (distance <= minDistance){
					growthPoints.erase(growthPoints.begin() + i);
					i--;
					pointRemoved = true;
					break;
				}
				//branch in range, determine if it is nearest
				else if (distance <= maxDistance){
					if (growthPoints.at(i).nearestBranch == nullptr){
						growthPoints.at(i).nearestBranch = b;
					}else if (length(growthPoints.at(i).nearestBranch->Position-growthPoints.at(i).position) > distance){
						growthPoints.at(i).nearestBranch = b;
					}
				}
			}
			//if the point wasn't removed
			if (!pointRemoved){
				//Set the grow parameters on all the closest branches that are in range
				if (growthPoints.at(i).nearestBranch != nullptr){
					vec3 dir = growthPoints.at(i).position - growthPoints.at(i).nearestBranch->Position;
					dir = normalize(dir);
//					cout<<length(dir)<<endl;
					growthPoints.at(i).nearestBranch->GrowDirection += dir;       //add to grow direction of branch
					growthPoints.at(i).nearestBranch->GrowCount++;
				}
			}
		}

		//Create new branches
		vector<Branch*> newBranches;
		newBranches.clear();
		for(Branch *b : m_branches){
			if(b->GrowCount>0){
//				cout<<"new branch"<<endl;
				vec3 avgDir = b->GrowDirection / b->GrowCount;
				avgDir = normalize(avgDir);
				//Hmmmmmm
				Branch *newBranch = new Branch(b, b->Position + (avgDir*branchLength), avgDir, b->top, b->top*branchShrink);

				newBranches.push_back(newBranch);
				b->Reset();
			}
		}

		bool branchAdded = false;
		for(Branch *newb : newBranches){
			bool contains = false;
			for(Branch *b : m_branches){
				if(b->Position==newb->Position){
					contains=true;
					break;
				}
			}
			if(!contains){
				m_branches.push_back(newb);
				branchAdded = true;
			}
		}
		if(m_branches.size()>=maxBranches){
			growing = false;
		}
		if(!branchAdded){
			growing = false;
		}
//		cout<<"Branches : "<<m_branches.size()<<endl;
	}
	cout<<m_branches.size()<<endl;
	createDisplayList();
}

void SpaceCoral::createDisplayList(){
  if (m_displayList)
    glDeleteLists(m_displayList, 1);
  m_displayList = glGenLists(1);
  glNewList(m_displayList, GL_COMPILE);
  glEnable(GL_LIGHTING);
  glPushMatrix();
	//glTranslatef(m_transPos.x, m_transPos.y, m_transPos.z);
	//glScalef(0.5f, 0.5f, 0.5f);
  int i = 1;
	//glColor3f(0.7f, 0.3f, 0.7f);
	for(Branch *b : m_branches){
//		glColor3f(i*0.1, i*0.1, i*0.1);
		DrawBranch(b->Position, b->OriginalGrowDirection, b->base, b->top, 5);
		i++;
	}
//	glBegin(GL_POINTS);
//	glColor3f(0.5f, 0.5f, 1.0f);
//	glPointSize(100);
//	glDisable(GL_LIGHTING);
//	for(GrowPoint g : growthPointsOrignal){
//	  glVertex3f(g.position.x, g.position.y, g.position.z);
//	}
//	glEnd();
	glPopMatrix();
	glDisable(GL_LIGHTING);
  glEndList();
}

void SpaceCoral::Draw(){
	glShadeModel(GL_SMOOTH);
	float col[3] = {color.r,color.g,color.b};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glCallList(m_displayList);
}

void SpaceCoral::DrawBranch(vec3 pos, vec3 dir, float base, float top, float slices){
	float radiusBase = base/2;
	float radiusTop = top/2;
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	vec3 dir2=normalize(-dir);
//	glLineWidth(2);
//	glBegin(GL_LINES);
//	glVertex3d(0,0,0);
//	glVertex3d(dir2.x*branchLength,dir2.y*branchLength,dir2.z*branchLength);
	float angle = degrees(acos(dot(vec3(0,0,1), dir2)));
	vec3 axis = cross(vec3(0,0,1),dir2);
	glRotatef(angle, axis.x, axis.y, axis.z);

	glBegin(GL_TRIANGLES);
	for(int i=0; i<slices; i++) {
		float theta = ((float)i / slices)*2.0*3.1459;
		float nextTheta = (((float)i + 1) / slices)*2.0*3.1459;
//		/*vertex at middle of end */
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusTop*cos(theta),radiusTop*sin(theta),branchLength);
		/*vertices at edges of circle*/
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f(radiusTop*cos(nextTheta), radiusTop*sin(nextTheta), branchLength);
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f (radiusBase*cos(nextTheta), radiusBase*sin(nextTheta), 0);
		/* the same vertices at the bottom of the cylinder*/
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusTop*cos(theta),radiusTop*sin(theta),branchLength);
		/*vertices at edges of circle*/
		glNormal3f(cos(theta), sin(theta), 0.0);
		glVertex3f(radiusBase*cos(theta), radiusBase*sin(theta), 0);
		glNormal3f(cos(nextTheta), sin(nextTheta), 0.0);
		glVertex3f (radiusBase*cos(nextTheta), radiusBase*sin(nextTheta), 0);

	}
	glEnd();
	glPopMatrix();
}

