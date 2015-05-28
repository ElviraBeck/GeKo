#include "GeKo_Gameplay/Input/MapTrackball.h"

MapTrackball::MapTrackball(Camera &activeCam)
{
	m_name = "Trackball";
	m_active = false;
	update(activeCam);
	m_type = MapType::CAMTRACKBALL;
}


MapTrackball::~MapTrackball()
{
}



// Fills the InputMaps specifically

void MapTrackball::update(Camera &activeCam){

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

	// Mapping the keys to the methods context specific
	auto trackball = [&](std::map<int, std::function<void()>> &m){
		m[GLFW_KEY_W] = cameraMoveFwd;
		m[GLFW_KEY_S] = cameraMoveBwd;
		m[GLFW_KEY_A] = cameraMoveLeft;
		m[GLFW_KEY_D] = cameraMoveRight;
		m[GLFW_KEY_E] = cameraMoveUp;
		m[GLFW_KEY_Q] = cameraMoveDown;
	};

	trackball(m_map);
}