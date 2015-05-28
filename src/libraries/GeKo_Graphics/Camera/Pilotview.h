#pragma once

#include "Camera.h"

/** Pilotview is a derived class of Camera.
It is able to change the position and turn the direction in all ways.
Every single movement is implemented per method which support key input as an input only*/

class Pilotview : public Camera
{
public:
	Pilotview(std::string name);
	~Pilotview();

	/// The method sets the position and updates the view matrix
	void setPosition(glm::vec4 position);

	/// The method changes the position in the way of m_direction
	void moveForward();

	/// The method changes the position in opposition to the way of m_direction
	void moveBackward();
	
	/// The method changes the position to the left on the axis, which is orthogonal to the direction
	void moveLeft();

	/// The method changes the position to the right on the axis, which is orthogonal to the direction
	void moveRight();

	/// The method changes the position upward on the axis, which is orthogonal to the direction
	void moveUp();

	/// The method changes the position downward on the axis, which is orthogonal to the direction
	void moveDown();

	/// The method changes viewing direction by turning left
	void turnLeft();

	/// The method changes viewing direction by turning right
	void turnRight();

	/// The method changes viewing direction by turning up
	void turnUp();

	/// The method changes viewing direction by turning down
	void turnDown();
};

