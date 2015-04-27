#include "GeKo_Gameplay/Questsystem/QuestHandler.h"

QuestHandler::QuestHandler(){
	m_questLog.push_back(new Quest());
	m_questGraph = new QuestGraph();
}

QuestHandler::~QuestHandler(){}

std::vector<Quest*> QuestHandler::getQuests()
{
	return m_questLog;
}

void QuestHandler::addQuest(Quest* q){
	m_questLog.push_back(q);
}

std::vector<Quest*> QuestHandler::getActiveAndNotFinished(){
	std::vector<Quest*> temp;
	for (int i = 0; i < m_questLog.size(); i++){
		if (m_questLog.at(i)->getActive() && !m_questLog.at(i)->getFinished()){
			temp.push_back(m_questLog.at(i));
		}
	}
	return temp;
}

std::vector<Goal*> QuestHandler::getQuests(GoalType gType){
	std::vector<Quest*> quests = getActiveAndNotFinished();
	std::vector<Goal*> goals;
	for (int i = 0; i < quests.size(); i++){
		for (int j = 0; j < quests.at(i)->getGoals().size(); j++){
			if (quests.at(i)->getGoals().at(j)->getGoalType() == gType){
				goals.push_back(quests.at(i)->getGoals().at(j));
			}
		}
	}
	return goals;
}

void QuestHandler::setGraph(QuestGraph* g){
	m_questGraph = g;

}

QuestGraph* QuestHandler::getGraph(){
	return m_questGraph;
}