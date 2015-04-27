#pragma once 

#include <iostream>
#include <glm/glm.hpp>

#include "GeKo_Gameplay/AI_Pathfinding/GraphNode.h"
#include "GeKo_Gameplay/AI_Pathfinding/AStarNode.h"
#include "GeKo_Gameplay/AI_Pathfinding/AStarAlgorithm.h"

#include "TreeOutput.h"
#include <GeKo_Gameplay/Object/States.h>

class DecisionTreeNode
{
public: 
	DecisionTreeNode(int nodeID, DecisionTreeNode* defaultNode);
	DecisionTreeNode(int nodeID, DecisionTreeNode* defaultNode, std::string name);
	DecisionTreeNode();
	~DecisionTreeNode();

	int getID();

	bool getIsLeaf();
	void setLeaf(bool isLeaf);

	States getStateID();
	void setStateID(States id);

	DecisionTreeNode* getLeftNode();
	void setLeftNode(DecisionTreeNode* left);

	DecisionTreeNode* getRightNode();
	void setRightNode(DecisionTreeNode* right);

	bool getCondition();
	void setCondition(bool con);

	TreeOutput getTarget();
	void setTarget(TreeOutput target);

	std::string getName();
	void setName(std::string name);

protected:
	int m_idNode;
	std::string m_name;

	DecisionTreeNode* m_leftNode;
	DecisionTreeNode* m_rightNode;

	bool m_isLeaf;
	
	TreeOutput m_target;

	States m_stateID;

	bool m_condition;
};