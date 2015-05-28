#pragma once

#include "Camera.h"

class StrategyCamera : public Camera
{

public:

	StrategyCamera(std::string name);

	~StrategyCamera();

	void turn(double xpos, double ypos);

	void setCenter(glm::vec4 center);
	void setRadius(float radius);

	void updateCursor(GLFWwindow* window);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void increaseRadius();
	void decreaseRadius();

	void setViewDirection(glm::vec3 viewDirection);
	
protected:
	double m_oldX, m_oldY;
	float m_sensitivity;
	double m_radius;
	//double angle;

	glm::vec3 m_movementVector;
	glm::vec3 m_viewDirection;
};