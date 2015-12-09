#include "mainheaders.hpp"
#include <vector>
#include "octree.hpp"
#include <iostream>
#include "boid.hpp"

using namespace std;
using namespace glm;

Octree::Octree(vec3 xyzMin, vec3 xyzMax){
	// Should add safety check, but not right now
	vec3 center = (xyzMax + xyzMin) / 2.f;
	vec3 halfSize = (xyzMax - xyzMin) / 2.f;

	root = new Node;
	root->center = center;
	root->halfSize = halfSize;
}

Octree::~Octree(){
	deleteTree(root);
	delete root;
}

void Octree::deleteTree(Node* n){
	if (n->bottom_NorthEast != nullptr){
		// Delete subtrees
		deleteTree(n->bottom_NorthEast);
		deleteTree(n->bottom_NorthWest);
		deleteTree(n->bottom_SouthEast);
		deleteTree(n->bottom_SouthWest);

		deleteTree(n->top_NorthEast);
		deleteTree(n->top_NorthWest);
		deleteTree(n->top_SouthEast);
		deleteTree(n->top_SouthWest);

		// Delete children nodes
		delete n->bottom_NorthEast;
		delete n->bottom_NorthWest;
		delete n->bottom_SouthEast;
		delete n->bottom_SouthWest;

		delete n->top_NorthEast;
		delete n->top_NorthWest;
		delete n->top_SouthEast;
		delete n->top_SouthWest;
	}
}

/*
* Inserts a new boid object into the tree
*/
bool Octree::insert(Boid* b){
	return insert(b, root);
}

/*
* Helper method to check whether in the bounds or not
*/
bool Octree::outsideBounds(vec3 point, vec3 xyzMin, vec3 xyzMax){
	// less than bounds
	if (point.x < xyzMin.x || point.y < xyzMin.y || point.z < xyzMin.z)
		return true;

	// greater than bounds
	if (point.x >= xyzMax.x || point.y >= xyzMax.y || point.z >= xyzMax.z)
		return true;

	return false;
}

/*
* Inserts a new boid object into the tree, creating a new branch if needed
*/
bool Octree::insert(Boid* b, Node* node){
	if (outsideBounds(b->getPosition(), node->center - node->halfSize, node->center + node->halfSize))
		return false;

	// If node is leaf and empty
	if (node->value == nullptr && node->top_NorthEast == nullptr){
		node->value = b;
		node->count++;
		return true;
	} else {
		// If leaf node with a value already or node is interior node
		if (node->top_NorthEast == nullptr){
			node->count--;
			subdivide(node);
		}

		// try to insert in bottom half
		if (insert(b, node->bottom_NorthEast)) { node->count++; return true; }
		if (insert(b, node->bottom_NorthWest)) { node->count++; return true; }
		if (insert(b, node->bottom_SouthEast)) { node->count++; return true; }
		if (insert(b, node->bottom_SouthWest)) { node->count++; return true; }

		// try to insert in top half
		if (insert(b, node->top_NorthEast)) { node->count++; return true; }
		if (insert(b, node->top_NorthWest)) { node->count++; return true; }
		if (insert(b, node->top_SouthEast)) { node->count++; return true; }
		if (insert(b, node->top_SouthWest)) { node->count++; return true; }
	}
	return false;
}

/*
* Subdivides node into 8 sub-branches
*/
void Octree::subdivide(Node* node){
	vec3 hc = node->halfSize / 2.f;
	// Create nodes
	node->top_NorthEast = new Node;
	node->top_NorthWest = new Node;
	node->top_SouthEast = new Node;
	node->top_SouthWest = new Node;

	node->bottom_NorthEast = new Node;
	node->bottom_NorthWest = new Node;
	node->bottom_SouthEast = new Node;
	node->bottom_SouthWest = new Node;

	// Set centers
	node->top_NorthEast->center = node->center + vec3(hc.x, -hc.y, hc.z);
	node->top_NorthWest->center = node->center + vec3(-hc.x, -hc.y, hc.z);
	node->top_SouthEast->center = node->center + vec3(hc.x, hc.y, hc.z);
	node->top_SouthWest->center = node->center + vec3(-hc.x, hc.y, hc.z);

	node->bottom_NorthEast->center = node->center + vec3(hc.x, -hc.y, -hc.z);
	node->bottom_NorthWest->center = node->center + vec3(-hc.x, -hc.y, -hc.z);
	node->bottom_SouthEast->center = node->center + vec3(hc.x, hc.y, -hc.z);
	node->bottom_SouthWest->center = node->center + vec3(-hc.x, hc.y, -hc.z);

	// set size
	node->top_NorthEast->halfSize = hc;
	node->top_NorthWest->halfSize = hc;
	node->top_SouthEast->halfSize = hc;
	node->top_SouthWest->halfSize = hc;

	node->bottom_NorthEast->halfSize = hc;
	node->bottom_NorthWest->halfSize = hc;
	node->bottom_SouthEast->halfSize = hc;
	node->bottom_SouthWest->halfSize = hc;

	// Moves value to one of the sub branches
	Boid* value = node->value;
	node->value = nullptr;
	insert(value, node);
}

/*
* Updates the position of boid b inside the tree
*/
void Octree::update(Boid* b){
	if (updateSub(b, root) || outsideBounds(b->getOldPosition(), root->center - root->halfSize, root->center + root->halfSize)){
		insert(b, root);
	}
}

/*
* Checks whether node is outside of bounds, and if so, reinserts into the correct space
*/
bool Octree::updateSub(Boid* b, Node* n){
	// check if in bounds
	if (outsideBounds(b->getOldPosition(), n->center - n->halfSize, n->center + n->halfSize))
		return false;

	// if leaf node, check value
	if (n->value != nullptr){
		if (&(*b) == &(*n->value) && outsideBounds(b->getPosition(), n->center - n->halfSize, n->center + n->halfSize)){
			n->value = nullptr;
			n->count--;
			return true;
		}
		else
			return false;
	}

	// Returns false if has no branches
	if (n->bottom_NorthEast == nullptr){
		return false;
	}

	bool shouldMove = false;
	// try to find in bottom half
	shouldMove |= updateSub(b, n->bottom_NorthEast);
	shouldMove |= updateSub(b, n->bottom_NorthWest);
	shouldMove |= updateSub(b, n->bottom_SouthEast);
	shouldMove |= updateSub(b, n->bottom_SouthWest);

	// try to find in top half
	shouldMove |= updateSub(b, n->top_NorthEast);
	shouldMove |= updateSub(b, n->top_NorthWest);
	shouldMove |= updateSub(b, n->top_SouthEast);
	shouldMove |= updateSub(b, n->top_SouthWest);

	// deletes the subtree if empty
	if (shouldMove){
		n->count--;
		deleteSubTree(n);
	}

	return shouldMove;
}
/*
* Removes boid b from the tree
*/
bool Octree::remove(Boid* b){
	return remove(b, root);
}

/*
* Removes boid b from the tree
*/
bool Octree::remove(Boid* b, Node* node){
	// If outside bounds, exit
	if (outsideBounds(b->getPosition(), node->center - node->halfSize, node->center + node->halfSize))
		return false;

	// If node is leaf and has boid
	if (node->value != nullptr){
		if (&(*node->value) == &(*b)){
			node->value = nullptr;
			return true;
		}
		return false;
	}
	else {
		bool hasDeleted = false;
		// try to remove from bottom half
		hasDeleted |= remove(b, node->bottom_NorthEast);
		hasDeleted |= remove(b, node->bottom_NorthWest);
		hasDeleted |= remove(b, node->bottom_SouthEast);
		hasDeleted |= remove(b, node->bottom_SouthWest);

		// try to remove from top half
		hasDeleted |= remove(b, node->top_NorthEast);
		hasDeleted |= remove(b, node->top_NorthWest);
		hasDeleted |= remove(b, node->top_SouthEast);
		hasDeleted |= remove(b, node->top_SouthWest);

		// Deletes sub tree if empty
		deleteSubTree(node);
		return hasDeleted;
	}
}

/*
* Deletes the subtree of node if the subtree is empty
*/
void Octree::deleteSubTree(Node* node){
	if (node->count == 0){
		// Delete bottom half
		delete node->bottom_NorthEast;
		node->bottom_NorthEast = nullptr;
		delete node->bottom_NorthWest;
		node->bottom_NorthWest = nullptr;
		delete node->bottom_SouthEast;
		node->bottom_SouthEast = nullptr;
		delete node->bottom_SouthWest;
		node->bottom_SouthWest = nullptr;

		// Delete top half
		delete node->top_NorthEast;
		node->top_NorthEast = nullptr;
		delete node->top_NorthWest;
		node->top_NorthWest = nullptr;
		delete node->top_SouthEast;
		node->top_SouthEast = nullptr;
		delete node->top_SouthWest;
		node->top_SouthWest = nullptr;
	}
}

/*
* Searches and returns the boids found in the region defined by xyzMin and xyzMax
*/
vector<Boid*>* Octree::searchRegion(vec3 xyzMin, vec3 xyzMax){
	vector<Boid*>* boids = new vector<Boid*>;
	queryRegion(boids, root, xyzMin, xyzMax);
	return boids;
}

/*
* Searches through the tree and fills the pasded in vector with boids found inside the region
*/
void Octree::queryRegion(std::vector<Boid*>* b, Node* n, vec3 xyzMin, vec3 xyzMax){
	// Returns if not within range
	if (n->center.x + n->halfSize.x < xyzMin.x || n->center.x - n->halfSize.x > xyzMax.x)
		return;
	if (n->center.y + n->halfSize.y < xyzMin.y || n->center.y - n->halfSize.y > xyzMax.y)
		return;
	if (n->center.z + n->halfSize.z < xyzMin.z || n->center.z - n->halfSize.z > xyzMax.z)
		return;

	// If leaf node then check if value in range
	if (n->bottom_NorthEast == nullptr){
		if (n->value != nullptr){
			if (outsideBounds(n->value->getPosition(), xyzMin, xyzMax))
				return;
			b->push_back(n->value);
		}
		return;
	}

	// Check sub branches
	queryRegion(b, n->bottom_NorthEast, xyzMin, xyzMax);
	queryRegion(b, n->bottom_NorthWest, xyzMin, xyzMax);
	queryRegion(b, n->bottom_SouthEast, xyzMin, xyzMax);
	queryRegion(b, n->bottom_SouthWest, xyzMin, xyzMax);

	queryRegion(b, n->top_NorthEast, xyzMin, xyzMax);
	queryRegion(b, n->top_NorthWest, xyzMin, xyzMax);
	queryRegion(b, n->top_SouthEast, xyzMin, xyzMax);
	queryRegion(b, n->top_SouthWest, xyzMin, xyzMax);
}

void Octree::debugDraw(){
	debugDrawBranch(root);
}


void Octree::debugDrawBranch(Node* n){
  	glUseProgram(0);
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	// Draw node
	glLineWidth(1);
	glBegin(GL_LINES);

	glColor3f(0.9, 0.9, 0.9);

	// top
	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);

	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);

	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);

	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);

	// bottom
	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);

	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);

	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);

	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);

	// Side
	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);

	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z - n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z - n->halfSize.z);

	glVertex3f(n->center.x + n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);
	glVertex3f(n->center.x + n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);

	glVertex3f(n->center.x - n->halfSize.x, n->center.y - n->halfSize.y, n->center.z + n->halfSize.z);
	glVertex3f(n->center.x - n->halfSize.x, n->center.y + n->halfSize.y, n->center.z + n->halfSize.z);
	glEnd();

	// Recurse through children
	if (n->bottom_NorthEast != nullptr){
		debugDrawBranch(n->bottom_NorthEast);
		debugDrawBranch(n->bottom_NorthWest);
		debugDrawBranch(n->bottom_SouthEast);
		debugDrawBranch(n->bottom_SouthWest);

		debugDrawBranch(n->top_NorthEast);
		debugDrawBranch(n->top_NorthWest);
		debugDrawBranch(n->top_SouthEast);
		debugDrawBranch(n->top_SouthWest);
	}
}
