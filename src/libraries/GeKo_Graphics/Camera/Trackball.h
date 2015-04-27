#pragma once

#include "Camera.h"

/** Trackball is a derived class of Camera.
It is able to change the position around the object but it is limited to move over the object in a half-sphere.
The direction view is always oriented towards the object.
Every single movement is implemented per method which support key input as an input only*/

class Trackball : public Camera
{
public:
	Trackball(std::string name);
	~Trackball();

	/// The method sets the position and updates the view matrix
	void setPosition(glm::vec4 position);
	
	/// The method sets the radius and updates the view matrix
	void setRadius(float radius);

	/// The method changes the position in the way of the object
	void moveForward();

	/// The method changes the position in opposition to the way of object
	void moveBackward();

	/// The method changes the positon in a circle around the object in the left way
	void moveLeft();

	/// The method changes the positon in a circle around the object in the right way
	void moveRight();

	/// The method changes the positon forwards in a half-circle over the object
	void moveUp();

	/// The method changes the positon backwards in a half-circle over the object
	void moveDown();

protected:
	// The radius determines the distance between the camera and the object during moving around it
	double m_radius;

	// Angles in x and y direction
	double m_phi, m_theta;
};