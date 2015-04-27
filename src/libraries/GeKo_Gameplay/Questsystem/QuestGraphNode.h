#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "GeKo_Gameplay/Questsystem/Quest.h"

/**Each QuestGraphNode contains a Quest-Object. It can have children which will be saved in a list and each QuestGraphNode can
have a parent-object.*/
class QuestGraphNode
{
public: 
	QuestGraphNode();
	~QuestGraphNode();

	///Returns the m_parent of the Node
	QuestGraphNode* getParent();
	void setParent(QuestGraphNode* p);

	///Returns the vector which contains the children
	std::vector<QuestGraphNode*> getChildren();
	void addChild(QuestGraphNode* c);

	///Returns the containing quest-object of the node
	Quest* getQuest();
	void setQuest(Quest* q);

protected:
	QuestGraphNode* m_parent;
	std::vector<QuestGraphNode*> m_children;

	Quest* m_quest;

};