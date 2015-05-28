#pragma once
#include "DirectionLight.h"

DirectionLight::DirectionLight()
{
	m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	m_isActive = true;
	m_direction = glm::vec3(0.0, -1.0, 0.0);
	m_radius = 0;
}

DirectionLight::DirectionLight(glm::vec4 position, glm::vec4 color, bool isActive, glm::vec3 direction, float radius)
{
	m_position = position;
	m_color = color;
	m_isActive = isActive;
	m_direction = direction;
	m_radius = radius;
}

DirectionLight::~DirectionLight()
{
}