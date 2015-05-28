#include "GeKo_Gameplay/Input/MapObject.h"

MapObject::MapObject()
{
	m_active = false;
	m_name = "Object";
	Player p;
	update(p);
	m_type = MapType::OBJECT;

	m_mutlipleKeys.push_back(GLFW_KEY_W);
	m_mutlipleKeys.push_back(GLFW_KEY_A);
	m_mutlipleKeys.push_back(GLFW_KEY_S);
	m_mutlipleKeys.push_back(GLFW_KEY_D);
	m_mutlipleKeys.push_back(GLFW_KEY_Q);
	m_mutlipleKeys.push_back(GLFW_KEY_E);
	m_mutlipleKeys.push_back(GLFW_KEY_F);
}

MapObject::~MapObject()
{
}

void MapObject::update(Player &p){

	// Setting all methods to move the player
	auto playerMoveFwd = [&](){
		p.moveForward();
	};

	auto playerMoveBwd = [&](){
		p.moveBackward();
	};

	auto playerMoveLeft = [&](){
		p.moveLeft();
	};

	auto playerMoveRight = [&](){
		p.moveRight();
	};

	auto playerTurnLeft = [&](){
		p.turnLeft();
	};

	auto playerTurnRight = [&](){
		p.turnRight();
	};

	// Setting all methods to open and close menues
	auto setFire = [&](){
		p.setFire();
	};

	auto inventory = [&](){
		//TODO: Zugriff auf GUI: Open or close inventory
	};

	auto journal = [&](){
		//TODO: Zugriff auf Quest Handler: Open or close Quest Handler
	};

	// Mapping the keys to the methods context specific
	auto action = [&](std::map<int, std::function<void()>> &m){
		//Mapping the keys to move methodes
		m[GLFW_KEY_W] = playerMoveFwd;
		m[GLFW_KEY_S] = playerMoveBwd;
		m[GLFW_KEY_A] = playerMoveLeft;
		m[GLFW_KEY_D] = playerMoveRight;
		m[GLFW_KEY_Q] = playerTurnLeft;
		m[GLFW_KEY_E] = playerTurnRight;
		//Mapping the keys to action methodes
		m[GLFW_KEY_F] = setFire;
		m[GLFW_KEY_I] = inventory;
		m[GLFW_KEY_J] = journal;
	};

	action(m_map);
}