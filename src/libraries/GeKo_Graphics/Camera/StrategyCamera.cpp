#include "StrategyCamera.h"

StrategyCamera::StrategyCamera(std::string name)
{
	m_name = name;

	m_movementVector = glm::vec3(0.0f);
	m_viewDirection = glm::vec3(0.0, 0.0, -1.0);

	m_position = glm::vec4(0.0f, 0.0f, 10.0f, 1.0);
	m_center = glm::vec4(0.5f, 0.5f, 0.5f, 1.0);
	m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0);

	m_width = 800;
	m_height = 600;

	m_fov = 60.0f;
	m_near = 0.001f;
	m_far = 100.0f;

	m_sensitivity = 0.010f;
	m_keySpeed = 2.0f;
	m_theta = glm::pi<float>() / 2.0f - 1;
	m_phi = -glm::pi<float>() / 2.0f;
	m_radius = 1.5;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	m_projectionMatrix = glm::perspective(m_fov, m_width / (float)m_height, m_near, m_far);

	m_oldX = m_width / 2;
	m_oldY = m_height / 2;
}

StrategyCamera::~StrategyCamera()
{

}
void StrategyCamera::updateCursor(GLFWwindow* window)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	m_oldX = x; 
	m_oldY = y;

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::turn(double xpos, double ypos)
{
	double changeX = (xpos - m_oldX)* m_sensitivity;
	double changeY = (ypos - m_oldY)* m_sensitivity;

	m_rotationAngle += glm::atan(changeX) * (180 / glm::pi<float>());

	m_theta -= changeY;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_phi -= changeX;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_oldX = xpos;
	m_oldY = ypos;

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewDirection.x = -m_radius * sin(m_theta) * cos(m_phi);
	m_viewDirection.y = m_radius * cos(m_theta);
	m_viewDirection.z = m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::setCenter(glm::vec4 center)
{
	m_center = center;
}

void StrategyCamera::setRadius(float radius)
{
	m_radius = radius;
}

void StrategyCamera::moveForward()
{

	m_movementVector += m_viewDirection;
	glm::vec2 newPos = glm::vec2(m_center.x, m_center.z) + glm::normalize(glm::vec2(m_movementVector.x, m_movementVector.z));
	m_movementVector = glm::vec3(0.0);

	m_center.x = newPos.x;
	m_center.z = newPos.y;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::moveBackward()
{
	m_movementVector -= m_viewDirection;
	glm::vec2 newPos = glm::vec2(m_center.x, m_center.z) + glm::normalize(glm::vec2(m_movementVector.x, m_movementVector.z));
	m_movementVector = glm::vec3(0.0);

	m_center.x = newPos.x;
	m_center.z = newPos.y;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::moveLeft()
{
	m_movementVector -= glm::cross(m_viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec2 newPos = glm::vec2(m_center.x, m_center.z) + glm::normalize(glm::vec2(m_movementVector.x, m_movementVector.z));
	m_movementVector = glm::vec3(0.0);

	m_center.x = newPos.x;
	m_center.z = newPos.y;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::moveRight()
{
	m_movementVector += glm::cross(m_viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec2 newPos = glm::vec2(m_center.x, m_center.z) + glm::normalize(glm::vec2(m_movementVector.x, m_movementVector.z));
	m_movementVector = glm::vec3(0.0);

	m_center.x = newPos.x;
	m_center.z = newPos.y;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::moveUp()
{
	m_center.y += m_keySpeed;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::moveDown()
{
	m_center.y -= m_keySpeed;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void StrategyCamera::increaseRadius()
{
		m_radius += 0.1f * m_keySpeed;
	if (m_radius > 10.0) m_radius = 10.0;
}

void StrategyCamera::decreaseRadius()
{
	m_radius -= 0.1f * m_keySpeed;
	if (m_radius < 1.0) m_radius = 1.0;
}


void StrategyCamera::setViewDirection(glm::vec3 viewDirection)
{

	double changeX = glm::dot(m_viewDirection, viewDirection);
	double changeY = glm::dot(m_viewDirection, viewDirection);

	m_theta -= changeY;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_phi -= changeX;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();


	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	
}