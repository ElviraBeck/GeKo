#include "GeKo_Gameplay/Input/MapPilotview.h"

MapPilotview::MapPilotview(Camera &activeCam)
{
	m_active = false;
	m_name = "Pilotview";
	update(activeCam);
	m_type = MapType::CAMPILOTVIEW;

	m_mutlipleKeys.push_back(GLFW_KEY_W);
	m_mutlipleKeys.push_back(GLFW_KEY_A);
	m_mutlipleKeys.push_back(GLFW_KEY_S);
	m_mutlipleKeys.push_back(GLFW_KEY_D);
}

MapPilotview::~MapPilotview()
{
}

void MapPilotview::update(Camera &activeCam){

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

	auto cameraMoveUp = [&](){
		activeCam.moveUp();
	};

	auto cameraMoveDown = [&](){
		activeCam.moveDown();
	};

	auto cameraTurnLeft = [&](){
		activeCam.turnLeft();
	};

	auto cameraTurnRight = [&](){
		activeCam.turnRight();
	};

	auto cameraTurnUp = [&](){
		activeCam.turnUp();
	};

	auto cameraTurnDown = [&](){
		activeCam.turnDown();
	};

	// Mapping the keys to the methods context specific
	auto pilotview = [&](std::map<int, std::function<void()>> &m){
		m[GLFW_KEY_W] = cameraMoveFwd;
		m[GLFW_KEY_S] = cameraMoveBwd;
		m[GLFW_KEY_A] = cameraMoveLeft;
		m[GLFW_KEY_D] = cameraMoveRight;
		m[GLFW_KEY_E] = cameraMoveUp;
		m[GLFW_KEY_Q] = cameraMoveDown;
		m[GLFW_KEY_LEFT] = cameraTurnLeft;
		m[GLFW_KEY_RIGHT] = cameraTurnRight;
		m[GLFW_KEY_UP] = cameraTurnUp;
		m[GLFW_KEY_DOWN] = cameraTurnDown;
	};

	pilotview(m_map);
}