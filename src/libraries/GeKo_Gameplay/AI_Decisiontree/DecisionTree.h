#pragma once 

#include "DecisionTreeNode.h"
#include "TreeOutput.h"
#include "GeKo_Gameplay/Object/States.h"

#include "GeKo_Gameplay/AI_Pathfinding/Graph.h"

#include <vector>
#include <glm/glm.hpp>

class DecisionTree
{
public:
	DecisionTree();
	DecisionTree(DecisionTreeNode* root);
	~DecisionTree();

	///This method goes through the tree and checks which branch it shall follow with the help of the actual state
	TreeOutput decide(std::vector<std::pair<States, bool>> states);

	void setRootNode(DecisionTreeNode &root);
	DecisionTreeNode* getRootNode();

	void setCurrentNode(DecisionTreeNode &current);
	DecisionTreeNode* getCurrentNode();

	bool getStates(std::vector<std::pair<States, bool>> list, States state);
	void setStates(std::vector<std::pair<States, bool>> list, States state, bool b);

	void setAntTreeAfraid();
	void setAntTreeAggressiv();

protected:
	DecisionTreeNode* m_rootNode;

	DecisionTreeNode* m_currentNode;
};