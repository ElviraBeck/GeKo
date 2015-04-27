#include "GeKo_Gameplay/Object/AntHome.h"

AntHome::AntHome(){

}

AntHome::AntHome(glm::vec3 position, AntMesh antMesh, ObjectObserver *objectObserver, Texture *guardTex, Texture *workerTex, DecisionTree *aggressiveDecisionTree, Graph<AStarNode, AStarAlgorithm> *aggressiveGraph, DecisionTree *afraidDecisionTree, Graph<AStarNode, AStarAlgorithm> *afraidGraph){
	m_position = position;
	m_antMesh = antMesh;
	//Texture texCV((char*)RESOURCES_PATH "/cv_logo.bmp");
	m_guardTexture = guardTex;
	m_workerTexture = workerTex;
	m_aggressiveGraph = aggressiveGraph;
	m_aggressiveDecisionTree = aggressiveDecisionTree;
	m_afraidGraph = afraidGraph;
	m_afraidDecisionTree = afraidDecisionTree;
	m_numberOfGuards = 0;
	m_numberOfWorkers = 0;
	m_objectObserver = objectObserver;
	m_gravity = new Gravity();
}

AntHome::~AntHome(){

}

void AntHome::generateGuards(int i, Node *root){
	if (i < 0)
		return;
	glm::vec4 position;
	time_t t;
	time(&t);
	srand((unsigned int)t);
	std::stringstream name;
	while (i > 0){
		name << "Guard" << m_numberOfGuards++ + 1;
		//m_numberOfGuards++;
		Node *aiGuardNode = new Node(name.str());
		aiGuardNode->addGeometry(&m_antMesh);
		aiGuardNode->addTexture(m_guardTexture);
		aiGuardNode->addGravity(m_gravity);
		position.x = rand() * 3 / 32767.0;
		position.z = rand() * 3 / 32767.0;
		position.y = 0.0;
		position.w = 0.0;
		Ant *antAI = new Ant(glm::vec4(m_position, 1.0) + position);
		//antAI.setAntAggressiv();
		antAI->setAntAggressiv(name.str(), m_aggressiveDecisionTree, m_aggressiveGraph);
		aiGuardNode->setObject(antAI);
		antAI->addObserver(m_objectObserver);
		name.str("");
		root->addChildrenNode(aiGuardNode);
		m_guards.push_back(aiGuardNode);
		i--;
		//printPosGuards();
	}
}


void AntHome::generateWorkers(int i, Node* root){
	if (i < 0)
		return;
	glm::vec4 position;
	time_t t;
	time(&t);
	srand((unsigned int)t);
	std::stringstream name;
	while (i > 0){
		name << "Worker" << m_numberOfWorkers++ + 1;
		Node *aiWorkerNode = new Node(name.str());
		aiWorkerNode->addGeometry(&m_antMesh);
		aiWorkerNode->addTexture(m_guardTexture);
		aiWorkerNode->addGravity(m_gravity);
		position.x = rand() * 5.0f / 32767.0f;
		position.z = rand() * 5.0f / 32767.0f;
		position.y = 10.0f;
		position.w = 0.0;
		Ant *antAI = new Ant(glm::vec4(m_position, 1.0) + position);
		antAI->setAntAfraid(name.str(), m_afraidDecisionTree, m_afraidGraph);
		//antAI->setAntAfraid();
		aiWorkerNode->setObject(antAI);
		antAI->addObserver(m_objectObserver);
		name.str("");
		root->addChildrenNode(aiWorkerNode);
		m_workers.push_back(aiWorkerNode);
		i--;
		//printPosWorkers();
	}
}

void AntHome::addAntsToSceneGraph(Node *rootNode){
	for (Node* antNode : m_guards){
		rootNode->addChildrenNode(antNode);
	}
	for (Node* antNode : m_workers){
		rootNode->addChildrenNode(antNode);
	}
}

void AntHome::updateAnts(){
	/*for (Node antNode : m_ants){
	antNode.getAI()->update();
	}*/
	for (int i = 0; i < m_guards.size(); i++){
		m_guards.at(i)->getAI()->update();
	}
	for (int i = 0; i < m_workers.size(); i++){
		m_workers.at(i)->getAI()->update();
	}
}

void AntHome::printPosGuards(){
	for (int i = 0; i < m_guards.size(); i++){
		std::cout << "Guard " << i << " Pos : x :" << m_guards[i]->getAI()->getPosition().x << " ; z: " << m_guards[i]->getAI()->getPosition().z << std::endl;
	}
}

void AntHome::printPosWorkers(){
	for (int i = 0; i < m_workers.size(); i++){
		std::cout << "Worker " << i << " Pos : x :" << m_workers[i]->getAI()->getPosition().x << " ; z: " << m_workers[i]->getAI()->getPosition().z << std::endl;
	}
}


