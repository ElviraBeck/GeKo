#pragma once

#include "GeKo_Gameplay/Object/AI.h"

class Ant : public AI
{
public:

	Ant();
	Ant(glm::vec4 position);
	~Ant();

	void decide();

	void updatePath();
	void updatePathPlayer();
	void updatePathPatrol();

	AStarNode* nextNodeOnPatrol();

	void setAntAfraid();
	void setAntAfraid(std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph);
	void setAntAggressiv();
	void setAntAggressiv(std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph);

protected:
	std::vector<AStarNode*> m_pathPatrol;
};