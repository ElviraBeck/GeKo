#pragma once
#include "GeKo_Gameplay/Object/Object.h"
#include "GeKo_Gameplay/Object/Ant.h"
#include "GeKo_Graphics/Geometry/AntMesh.h"
#include "GeKo_Graphics/Scenegraph/Node.h"
#include "GeKo_Graphics/Material/Texture.h"
#include "GeKo_Gameplay/Observer/ObjectObserver.h"
#include "GeKo_Gameplay/Observer/GravityObserver.h"
#include "GeKo_Gameplay/AI_Decisiontree/DecisionTree.h"
#include "GeKo_Gameplay/AI_Pathfinding/Graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <vector>

class AntHome : public Object{
public:
	AntHome();
	AntHome(glm::vec3 position, AntMesh antMesh, ObjectObserver *objectObserver, Texture *guardTex, Texture *workerTex, DecisionTree *aggressiveDecisionTree, Graph<AStarNode, AStarAlgorithm> *aggressiveGraph, DecisionTree *afraidDecisionTree, Graph<AStarNode, AStarAlgorithm> *afraidGraph);
	~AntHome();
	void generateGuards(int i, Node* root);
	void generateWorkers(int i);
	void generateWorkers(int i, Node* root);


	void updateAnts();
	void addAntsToSceneGraph(Node *rootNode);
	//void putObserver();
	void printPosGuards();
	void printPosWorkers();

protected:
	std::vector<Node*> m_guards;
	std::vector<Node*> m_workers;
	glm::vec3 m_position;
	AntMesh m_antMesh;
	Texture *m_guardTexture;
	Texture *m_workerTexture;
	Gravity *m_gravity;
	Graph<AStarNode, AStarAlgorithm> *m_aggressiveGraph;
	DecisionTree *m_aggressiveDecisionTree;
	Graph<AStarNode, AStarAlgorithm> *m_afraidGraph;
	DecisionTree *m_afraidDecisionTree;
	int m_numberOfGuards;
	int m_numberOfWorkers;

	ObjectObserver *m_objectObserver;
};