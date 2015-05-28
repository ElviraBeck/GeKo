#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

/** Camera is a superclass of the specific camera classes*/

class Camera{

public:
	/// Standard constructor: Camera is set and a standard name is given
	Camera(std::string name = "Camera");
	~Camera();

	void virtual update(GLFWwindow* window);

	/// This method sets the FOV
	void setFOV(float fov);

	/// This method sets the lookAt-Vector
	void setLookAt(glm::vec3 lookAt);

	/// This method returns the m_viewMatrix
	glm::mat4 getViewMatrix();

	/// This method returns the m_projectionMatrix
	glm::mat4 getProjectionMatrix();

	/// This method sets the near and far plane
	void setNearFar(float near, float far);

	/// This method returns the near value m_near
	float getNear();

	/// This method returns the far value m_far
	float getFar();

	/// This method sets the width and height of the viewport
	void setWidthHeight(int width, int height);

	//This method sets the position of the camera
	void setPosition(glm::vec4 pos);

	/// This method returns the position m_position of the camera
	glm::vec4 getPosition();

	/// This method returns the name of the camera
	std::string getName();

	/// This method sets the name of the camera
	void setName(std::string n);

	/// This method returns the sensitivity
	/** The sensitivity is the framerate. The delta time has to be computed in the main.
	*/
	float getSensitivity();

	/// This method sets the delta time
	void setSensitivity(float sensitivity);

	/// This method returns the m_keySpeed value
	/** The  key speed represents the increment, which is different from the mouse speed
	*/
	float getKeySpeed();

	/// This method sets m_keySpeed
	void setKeySpeed(float speed);

	/// This method returns the m_mouseSpeed value
	/** The  mouse speed represents the increment, which is different from the key speed
	*/
	float getMouseSpeed();

	/// This method sets m_mouseSpeed
	void setMouseSpeed(float speed);

	/// This method return the look at m_center
	glm::vec4 getCenter();
	/// This method set the m_center
	void setCenter(glm::vec4 c);

	/// This method returns the angle phi
	/** Phi is the angle in x direction
	*/
	float getPhi();
	/// This method returns the angle theta
	/** Theta is the angle in y direction
	*/
	float getTheta();

	double getRotationAngle();

	// Implemented in the specific cameras
	virtual void moveForward();
	virtual void moveBackward();
	virtual void moveLeft();
	virtual void moveRight();
	virtual void moveUp();
	virtual void moveDown();
	virtual void turnLeft();
	virtual void turnRight();
	virtual void turnUp();
	virtual void turnDown();
	//virtual void turn(GLFWwindow* window);
	virtual void turn(double xpos, double ypos);
	virtual void increaseRadius();
	virtual void decreaseRadius();
	virtual void updateCursor(GLFWwindow* window);


protected:
	// The name identifies the camera
	std::string m_name;

	// The field of view, near and far plane are needed to compute the perspective
	float m_fov, m_near, m_far;

	// The width and height defines the size of the viewport
	int m_width, m_height;

	// The sensitivity is used to store the framerate and is computed through delta time
	float m_sensitivity;

	// The view matrix will transform vertices from world-space to view-space
	glm::mat4 m_viewMatrix;

	// The projection matrix transformes vertices into the clip-space
	glm::mat4 m_projectionMatrix;

	// Position, center(look at) and up are needed to compute the view matrix
	glm::vec4 m_position, m_center, m_up;

	// Speed of movement by using keys
	float m_keySpeed;

	// Speed of movement by using the mouse
	float m_mouseSpeed;

	// m_direction defines the viewing direction
	glm::vec4 m_direction;

	// Angles in x(phi) and y(theta) direction
	double m_phi, m_theta;
	// Angle step
	float m_rotationAngle;
};