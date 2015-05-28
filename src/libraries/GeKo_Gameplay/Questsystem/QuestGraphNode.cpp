#include "GeKo_Gameplay/Questsystem/QuestGraphNode.h"

QuestGraphNode::QuestGraphNode(){
}

QuestGraphNode::~QuestGraphNode(){}

void QuestGraphNode::setParent(QuestGraphNode* p){
	m_parent = p;
	p->addChild(this);
}

QuestGraphNode* QuestGraphNode::getParent(){
	return m_parent;
}

void QuestGraphNode::addChild(QuestGraphNode* c){
	m_children.push_back(c);
}

std::vector<QuestGraphNode*> QuestGraphNode::getChildren(){
	return m_children;
}

Quest* QuestGraphNode::getQuest(){
	return m_quest;
}

void QuestGraphNode::setQuest(Quest* q){
	m_quest = q;
}