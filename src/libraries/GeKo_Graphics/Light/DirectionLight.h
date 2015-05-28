#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/*
DirectionLight is one type of Light.
It has color, position, radius, direction and you can switch the light on and off with m_isActive.
If radius is 0, then its "infinite".

DirectionLight has position only for representation, it has no influence for shading.

Exponent and angle is always 0.
*/
class DirectionLight
{
public:
	/*
	Standard constructor - set an active light with position (0,0,0,1), color (1,1,1,1), direction (0,-1,0) and radius 0/infinite
	*/
	DirectionLight();
	DirectionLight(glm::vec4 position, glm::vec4 color, bool isActive, glm::vec3 direction, float radius);
	~DirectionLight();

	glm::vec4 m_color;
	glm::vec4 m_position;
	bool m_isActive;
	glm::vec3 m_direction;
	float m_radius;

	float m_exponent = 0;
	float m_angle = 0;
};

