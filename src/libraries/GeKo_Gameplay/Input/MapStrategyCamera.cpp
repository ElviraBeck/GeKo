#include "GeKo_Gameplay/Input/MapStrategyCamera.h"

MapStrategyCamera::MapStrategyCamera(Camera &activeCam)
{
	m_name = "Strategy";
	m_active = false;
	update(activeCam);
	m_type = MapType::CAMSTRATEGY;
}


MapStrategyCamera::~MapStrategyCamera()
{
}



// Fills the InputMaps specifically

void MapStrategyCamera::update(Camera &activeCam){

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

	auto update = [&](){
		activeCam.update(getWindow());

	};

	auto moveRadiusMinus = [&]()
	{
		activeCam.increaseRadius();
	};

	auto moveRadiusPlus = [&]()
	{
		activeCam.decreaseRadius();
	};

	// Mapping the keys to the methods context specific
	auto strategy = [&](std::map<int, std::function<void()>> &m){
		m[GLFW_KEY_W] = cameraMoveFwd;
		m[GLFW_KEY_S] = cameraMoveBwd;
		m[GLFW_KEY_A] = cameraMoveLeft;
		m[GLFW_KEY_D] = cameraMoveRight;
		m[GLFW_KEY_E] = cameraMoveUp;
		m[GLFW_KEY_Q] = cameraMoveDown;
		m[GLFW_MOUSE_BUTTON_1] = update;
		m[001] = moveRadiusMinus;
		m[002] = moveRadiusPlus;
	};

	strategy(m_map);
}