#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <glm/ext.hpp>
#include "GeKo_Gameplay/AI_Pathfinding/GraphNode.h"

/** This class is for the AStarAlgorithm. It builds a whole new world of graphnodes which nobody needs.*/
class AStarNode : public GraphNode<AStarNode>
{

public: 
	///Every GraphNode needs a name
	/**/
	AStarNode(std::string name, AStarNode *defaultNode);
	AStarNode();
	AStarNode(std::string name, AStarNode *defaultNode, glm::vec3 position, GraphNodeType type);
	~AStarNode();
	
	///Sets the m_distanceToGoal to distance
	/**/
	void setDistanceToGoal(float distance);
	///Returns m_distanceToGoal
	/**/
	float getDistanceToGoal();

	///Returns m_distanceTravelled
	/**/
	float getDistanceTravelled();
	///Sets m_distanceTravelled to distance
	/**/
	void setDistanceTravelled(float distance);

	///Sets m_wasVisitedBy to visitor
	/**/
	void setVisitor(AStarNode* visitor);
	///Return m_wasVisitedBy
	/**/
	AStarNode* getVisitor();

	///Sets m_temporaryTravelled to temp
	/**/
	void setTemporary(float temp);
	///Returns m_temporaryTravelled 
	/**/
	float getTemporary();

	///Returns m_default Node 
	/**/
	AStarNode* getDefault();
	void setDefault(AStarNode* d);
	
protected:
	float m_distanceToGoal;

	float m_distanceTravelled;
	float m_temporaryTravelled;

	AStarNode* m_wasVisitedBy;

	AStarNode* m_default;
};