#include "GeKo_Gameplay/Input/MapPlayerview.h"

MapPlayerview::MapPlayerview(Camera &activeCam)
{
	m_active = false;
	m_name = "Playerview";
	update(activeCam);
	m_type = MapType::CAMPLAYERVIEW;
}

MapPlayerview::~MapPlayerview()
{
}

void MapPlayerview::update(Camera &activeCam){

	// Setting all methods
	auto cameraMoveFwd = [&](){
		activeCam.moveForward();
	};

	auto cameraMoveBwd = [&](){
		activeCam.moveBackward();
	};

	auto cameraMoveLeft = [&](){
		activeCam.moveLeft();
	};

	auto cameraMoveRight = [&](){
		activeCam.moveRight();
	};

	// Mapping the keys to the methods context specific
	auto playerview = [&](std::map<int, std::function<void()>> &m){
		m[GLFW_KEY_W] = cameraMoveFwd;
		m[GLFW_KEY_S] = cameraMoveBwd;
		m[GLFW_KEY_A] = cameraMoveLeft;
		m[GLFW_KEY_D] = cameraMoveRight;
	};

	playerview(m_map);
}