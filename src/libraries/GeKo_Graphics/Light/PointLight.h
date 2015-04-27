#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/*
PointLight is one type of Light.
It has position, color, radius and you can switch the light on and off with m_isActive.
If radius is 0, then its "infinite".

Direction, exponent and angle is always 0.
*/
class PointLight 
{
public:
	/*
	Standard constructor - set an active light with position (0,0,0,1), color (1,1,1,1), radius 0/infinite
	*/
	PointLight();
	PointLight(glm::vec4 position, glm::vec4 color, bool isActive, float radius);
	~PointLight();

	glm::vec4 m_color;
	glm::vec4 m_position;
	bool m_isActive;
	float m_radius;

	glm::vec3 m_direction = glm::vec3(0.0, 0.0, 0.0);
	float m_exponent = 0;
	float m_angle = 0;
};