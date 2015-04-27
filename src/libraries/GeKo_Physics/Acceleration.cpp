#include "Acceleration.h"

	Acceleration::Acceleration(){
	    m_acceleration = 0.0001;
		maxSpeed = 1.0;
		m_speed = 0.0;
	}
	
	Acceleration::Acceleration(double accel, double max){
		m_acceleration = accel;
		maxSpeed = max;
		m_speed = 0.0;
	}
	
	Acceleration::~Acceleration(){
	
	}
	
	void Acceleration::setAcceleration(double newAccel){
		m_acceleration = newAccel;
	}
	
	double Acceleration::getAcceleration(){
		return m_acceleration;
	}
	
	void Acceleration::accelerate(){
		if ((m_speed <= maxSpeed) && (m_speed >= 0)){
		m_speed += m_acceleration;
		}
	}

	void Acceleration::setSpeed(double speed)
	{
		m_speed = speed;
	}

	double Acceleration::getSpeed()
	{
		return m_speed;
	}