#pragma once

#include "Camera.h"

/** Playerview is a derived class of Camera.
It is able to change the position and turn the direction in all ways.
The class supports key input to change position and mouse input to turn directions.*/

class Playerview : public Camera
{
public:
	Playerview(std::string name);
	~Playerview();

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

	/// The method changes direction with the help of the mouse position callback
	void turn(double xpos, double ypos);

protected:
	// m_oldX and m_oldY remember the last x and y value of the mouse position
	double m_oldX, m_oldY;
};