#include "DecisionTreeNode.h"

DecisionTreeNode::DecisionTreeNode(int nodeID, DecisionTreeNode* defaultNode){
	m_idNode = nodeID;
	m_name = "Default";

	m_isLeaf = false;
	
	m_stateID = States::DEFAULTSTATE;

	m_leftNode = defaultNode;
	m_rightNode = defaultNode;

	m_target = TreeOutput::HOME;

	m_condition = NULL;
}

DecisionTreeNode::DecisionTreeNode(int nodeID, DecisionTreeNode* defaultNode, std::string name){
	m_idNode = nodeID;
	m_name = name;

	m_isLeaf = false;

	m_stateID = States::DEFAULTSTATE;

	m_leftNode = defaultNode;
	m_rightNode = defaultNode;

	m_target = TreeOutput::HOME;

	m_condition = NULL;
}

DecisionTreeNode::DecisionTreeNode(){
}

DecisionTreeNode::~DecisionTreeNode(){
}

int DecisionTreeNode::getID(){
	return m_idNode;
}

bool DecisionTreeNode::getIsLeaf(){
	return m_isLeaf;
}

void DecisionTreeNode::setLeaf(bool isLeaf){
	m_isLeaf = isLeaf;
}

States DecisionTreeNode::getStateID(){
	return m_stateID;
}

void DecisionTreeNode::setStateID(States id){
	m_stateID = id;
}

DecisionTreeNode* DecisionTreeNode::getLeftNode(){
	return m_leftNode;
}

void DecisionTreeNode::setLeftNode(DecisionTreeNode* left){
	m_leftNode = left;
}

DecisionTreeNode* DecisionTreeNode::getRightNode(){
	return m_rightNode;
}

void DecisionTreeNode::setRightNode(DecisionTreeNode* right){
	m_rightNode = right;
}

bool DecisionTreeNode::getCondition(){
	return m_condition;
}

void DecisionTreeNode::setCondition(bool con){
	m_condition = con;
}

TreeOutput DecisionTreeNode::getTarget(){
	return m_target;
}

 void DecisionTreeNode::setTarget(TreeOutput target){
	m_target = target;
}

 std::string DecisionTreeNode::getName(){
	 return m_name;
 }

 void DecisionTreeNode::setName(std::string name){
	 m_name = name;
 }