#include "mainheaders.hpp"
#include <vector>

class Boid;

class Octree{
private:
	typedef struct Node{
		// Dimensions of this node
		glm::vec3 center;
		glm::vec3 halfSize;

		// Value this leaf holds
		Boid *value = nullptr;

		// number of values in the node and subnodes
		int count = 0;

		// Top layer
		Node *top_NorthWest = nullptr;
		Node *top_NorthEast = nullptr;
		Node *top_SouthWest = nullptr;
		Node *top_SouthEast = nullptr;

		// Bottom layer
		Node *bottom_NorthWest = nullptr;
		Node *bottom_NorthEast = nullptr;
		Node *bottom_SouthWest = nullptr;
		Node *bottom_SouthEast = nullptr;
	};

	Node* root;
	bool insert(Boid *b, Node* n);
	void subdivide(Node* node);
	bool updateSub(Boid *b, Node* n);
	bool remove(Boid *b, Node* node);
	void deleteSubTree(Node* node);
	bool outsideBounds(glm::vec3 point, glm::vec3 xyzMin, glm::vec3 xyzMax);
	void queryRegion(std::vector<Boid*>*, Node* n, glm::vec3 xyzMin, glm::vec3 xyzMax);
	void deleteTree(Node* n);
	void debugDrawBranch(Node* n);
public:
	Octree(glm::vec3 xyzMin, glm::vec3 xyzMax);
	~Octree();

	bool insert(Boid *b);
	void update(Boid *b);
	bool remove(Boid *b);
	void debugDraw();
	std::vector<Boid*>* searchRegion(glm::vec3 xyzMin, glm::vec3 xyzMax);
};
