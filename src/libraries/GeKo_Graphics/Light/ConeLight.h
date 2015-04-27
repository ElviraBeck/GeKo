#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/*
ConeLight is one type of Light.
It has position, color, direction, angle, exponent radius and you can switch the light on and off with m_isActive.
If radius is 0, then its "infinite".

If angle is 0, then it looks like a Point- or DirectionLight
*/
class ConeLight
{
public:
	/*
	Standard constructor - set an active light with position (0,0,0,1), color (1,1,1,1), direction (1,0,0), angle 90, exponent 1 and radius 0/infinite
	*/
	ConeLight();
	ConeLight(glm::vec4 position, glm::vec4 color, bool isActive, glm::vec3 direction, float angle, float exponent, float radius);
	~ConeLight();

	glm::vec4 m_color;
	glm::vec4 m_position;
	bool m_isActive;
	glm::vec3 m_direction;
	float m_exponent;
	float m_angle;
	float m_radius;
};