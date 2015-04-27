#pragma once
#include "ConeLight.h"

ConeLight::ConeLight()
{
	m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);
	m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	m_isActive = true;
	m_direction = glm::vec3(1.0, 0.0, 0.0);
	m_angle = 90;
	m_exponent = 1.0;
	m_radius = 0;
}

ConeLight::ConeLight(glm::vec4 position, glm::vec4 color, bool isActive, glm::vec3 direction, float angle, float exponent, float radius)
{
	m_position = position;
	m_color = color;
	m_isActive = isActive;
	m_direction = direction;
	m_angle = angle;
	m_exponent = exponent;
	m_radius = radius;
}

ConeLight::~ConeLight()
{
}