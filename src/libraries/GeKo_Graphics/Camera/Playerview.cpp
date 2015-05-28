#include "Playerview.h"

Playerview::Playerview(std::string name){

	m_name = name;

	m_position = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
	m_direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	m_theta = glm::pi<float>() / 2.0f;
	m_phi = glm::pi<float>() / 2.0f;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));
	m_projectionMatrix = glm::perspective(m_fov, m_width / (float)m_height, m_near, m_far);

	m_oldX = m_width / 2;
	m_oldY = m_height / 2;

	m_mouseSpeed = 10.0f;//0.1f;
	m_keySpeed = 50.0f;//2.0f;
	m_sensitivity = 0.01f;

	m_rotationAngle = 0;
}

Playerview::~Playerview()
{
}

void Playerview::setPosition(glm::vec4 position){
	m_position = position;
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));
}

void Playerview::moveForward(){
	m_position.x += m_keySpeed* m_sensitivity*m_direction.x;
	m_position.y += m_keySpeed* m_sensitivity*m_direction.y;
	m_position.z += m_keySpeed* m_sensitivity*m_direction.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveForward()" << std::endl;
}

void Playerview::moveBackward(){
	m_position.x -= m_keySpeed* m_sensitivity*m_direction.x;
	m_position.y -= m_keySpeed* m_sensitivity*m_direction.y;
	m_position.z -= m_keySpeed* m_sensitivity*m_direction.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveBackward()" << std::endl;
}

void Playerview::moveLeft(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_direction.x, m_direction.y, m_direction.z), glm::vec3(m_up.x, m_up.y, m_up.z));
	m_position.x -= m_keySpeed* m_sensitivity*directionOrtho.x;
	m_position.y -= m_keySpeed* m_sensitivity*directionOrtho.y;
	m_position.z -= m_keySpeed* m_sensitivity*directionOrtho.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveLeft()" << std::endl;
}

void Playerview::moveRight(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_direction.x, m_direction.y, m_direction.z), glm::vec3(m_up.x, m_up.y, m_up.z));
	m_position.x += m_keySpeed* m_sensitivity*directionOrtho.x;
	m_position.y += m_keySpeed* m_sensitivity*directionOrtho.y;
	m_position.z += m_keySpeed* m_sensitivity*directionOrtho.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveRight()" << std::endl;
}

void Playerview::turn(double xpos, double ypos){

	double changeX =( xpos - m_oldX) *( m_sensitivity* m_mouseSpeed);
	double changeY = (ypos - m_oldY) *( m_sensitivity* m_mouseSpeed);
	
	m_rotationAngle += glm::atan(changeX) * (180 / glm::pi<float>());

	m_theta -= changeY;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_phi -= changeX;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_direction.x = sin(m_theta) * cos(m_phi);
	m_direction.y = -cos(m_theta);
	m_direction.z = -sin(m_theta) * sin(m_phi);
		
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	m_oldX = xpos;
	m_oldY= ypos;
}