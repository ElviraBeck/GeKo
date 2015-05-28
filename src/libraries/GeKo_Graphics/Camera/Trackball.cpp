#include "Trackball.h"

Trackball::Trackball(std::string name){

	m_name = name;

	m_position = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
	m_center = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	m_radius = 10;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	m_projectionMatrix = glm::perspective(m_fov, (float)m_width / (float)m_height, m_near, m_far);

	m_theta = glm::pi<float>() / 2.0f;
	m_phi = -glm::pi<float>() / 2.0f;

	m_keySpeed = 20.0;//2.0;
	m_sensitivity = 0.01;

	m_direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

	m_rotationAngle = 0;
}

Trackball::~Trackball()
{
}

void Trackball::setPosition(glm::vec4 position){
	m_position = position;
	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void Trackball::setRadius(float radius){
	m_radius = radius;
}

void Trackball::moveForward(){

	if (m_radius <= 1){
		m_radius = 1;
	}
	else {
		m_direction = glm::normalize(m_center - m_position);

		m_position.x += m_keySpeed*m_sensitivity*m_direction.x;
		m_position.y += m_keySpeed*m_sensitivity*m_direction.y;
		m_position.z += m_keySpeed*m_sensitivity*m_direction.z;
		m_radius -= m_keySpeed*m_sensitivity;

		m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	
		std::cout << "moveForward() " << std::endl;
	}
}

void Trackball::moveBackward(){

	m_direction = glm::normalize(m_center - m_position);

	m_position.x -= m_keySpeed*m_sensitivity*m_direction.x;
	m_position.y -= m_keySpeed*m_sensitivity*m_direction.y;
	m_position.z -= m_keySpeed*m_sensitivity*m_direction.z;
	m_radius += m_keySpeed*m_sensitivity;

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));

	std::cout << "moveBackward() " << std::endl;
}

void Trackball::moveLeft(){
	m_rotationAngle = -m_keySpeed* m_sensitivity;
	

	m_phi -= m_keySpeed* m_sensitivity;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	
	std::cout << "moveLeft()" << std::endl;
}

void Trackball::moveRight(){
	m_rotationAngle = m_keySpeed* m_sensitivity;

	m_phi += m_keySpeed* m_sensitivity;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));

	std::cout << "moveRight()" << std::endl;
}

void Trackball::moveUp(){
	m_theta -= m_keySpeed* m_sensitivity;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));

	std::cout << "moveUp()" << std::endl;
}

void Trackball::moveDown(){
	m_theta += m_keySpeed* m_sensitivity;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_position.x = m_radius * sin(m_theta) * cos(m_phi);
	m_position.y = m_radius * cos(m_theta);
	m_position.z = -m_radius * sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_center) + glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));

	std::cout << "moveDown()" << std::endl;
}