#pragma once
# include "GeKo_Gameplay/AI_Pathfinding/Algorithm.h"
#include "GeKo_Gameplay/AI_Pathfinding/AStarNode.h"
#include <iostream>

/**Provides the A*-Algorithm to calculate the shortest path between two given points.*/
class AStarAlgorithm : public Algorithm<AStarNode>
{
public:

	AStarAlgorithm(std::string name);
	~AStarAlgorithm();

	 AStarNode* startAlgorithm (AStarNode* startNode, AStarNode* endNode);

	 ///Returns the fastest path from Start to Goal
	 /**The vector will contain nodes. The startNode will be at the end of the vector list*/
	std::vector<AStarNode*>* startAlgorithm2(AStarNode* startNode, AStarNode* endNode, std::vector<AStarNode*> &path);

private:
	std::vector<AStarNode*> m_path;
};