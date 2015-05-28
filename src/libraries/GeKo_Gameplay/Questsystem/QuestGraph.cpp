#include "GeKo_Gameplay/Questsystem/QuestGraph.h"

QuestGraph::QuestGraph(){
	
	m_hasChildren = false;
}

QuestGraph::~QuestGraph(){}


std::vector<QuestGraphNode*> QuestGraph::getNodes(){
	return m_nodes;
}

void QuestGraph::addNode(QuestGraphNode* n){
	m_hasChildren = true;
	m_nodes.push_back(n);
}

QuestGraphNode* QuestGraph::search(Quest* quest){
	QuestGraphNode* tmp;
	for (QuestGraphNode* n : m_nodes){
		if (n->getQuest()->getID() == quest->getID()){
			tmp =  n;
		}
	}
	return tmp;
}

QuestGraph* QuestGraph::setExampleGraph(){
	Quest questCollect(1);
	questCollect.setActive(true);
	Goal_Collect goalCollect(1);
	Goal_Collect goalCollect3(3);
	questCollect.addGoal(&goalCollect);
	questCollect.addGoal(&goalCollect3);

	Quest questCollect2(2);
	questCollect2.setActive(true);
	Goal_Collect goalCollect2(2);
	questCollect2.addGoal(&goalCollect2);

	Quest questCollect3(3);
	questCollect3.setActive(false);
	Goal_Collect gC4(4);
	questCollect3.addGoal(&gC4);

	goalCollect.setGoalCount(50);
	goalCollect2.setGoalCount(50);
	goalCollect3.setGoalCount(50);
	gC4.setGoalCount(50);

	goalCollect.setItemType(ItemType::BRANCH);
	goalCollect2.setItemType(ItemType::BRANCH);
	goalCollect3.setItemType(ItemType::COOKIE);
	gC4.setItemType(ItemType::COOKIE);

	QuestGraphNode node1;
	QuestGraphNode node2;
	QuestGraphNode node3;

	node1.setQuest(&questCollect);
	node2.setQuest(&questCollect2);
	node3.setQuest(&questCollect3);

	node1.addChild(&node2);
	node1.addChild(&node3);

	addNode(&node1);
	addNode(&node2);
	addNode(&node3);

	return this;
}

bool QuestGraph::hasChildren()
{
	return m_hasChildren;
}