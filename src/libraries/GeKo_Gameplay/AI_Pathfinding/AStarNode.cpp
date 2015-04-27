#include "GeKo_Gameplay/AI_Pathfinding/AStarNode.h"

AStarNode::AStarNode(std::string name, AStarNode *defaultNode) :
GraphNode(name)	
{
	setVisitor(defaultNode);
	setDefault(defaultNode);
	m_distanceToGoal = 0;
	m_distanceTravelled = 0;
}

AStarNode::AStarNode() :
GraphNode("Default")
{
	m_name = "Default";
	m_distanceToGoal = 0;
	m_distanceTravelled = 0;
}

AStarNode::AStarNode(std::string name,AStarNode *defaultNode, glm::vec3 position, GraphNodeType type) :
GraphNode(name)
{
	setVisitor(defaultNode);
	setDefault(defaultNode);
	setNodeType(type);
	setPosition(position);
	m_distanceToGoal = 0;
	m_distanceTravelled = 0;
}

AStarNode::~AStarNode()
{

}

void AStarNode::setDistanceToGoal(float distance)
{
	m_distanceToGoal = distance;
}

float AStarNode::getDistanceToGoal()
{
	return m_distanceToGoal;
}

float AStarNode::getDistanceTravelled()
{
	return m_distanceTravelled;
}

void AStarNode::setDistanceTravelled(float distance)
{
	m_distanceTravelled += distance;
}

void AStarNode::setVisitor(AStarNode* visitor)
{
	m_wasVisitedBy = visitor;
}

AStarNode* AStarNode::getVisitor()
{
	return m_wasVisitedBy;
}

void AStarNode::setTemporary(float temp)
{
	m_temporaryTravelled = temp;
}

float AStarNode::getTemporary()
{
	return m_temporaryTravelled;
}

AStarNode* AStarNode::getDefault()
{
	return m_default;
}

void AStarNode::setDefault(AStarNode* d){
	m_default = d;
}