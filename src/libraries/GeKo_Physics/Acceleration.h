#pragma once

///A class to add acceleration to an  object.
/** The speed is added each frame to the object untill it reaches a maximum speed, so that it seems the objects gets faster gradually.
*/
class Acceleration{
protected:
	double m_acceleration;
	double m_speed; 		 
	double maxSpeed;    
	
public:
	Acceleration();
	Acceleration(double accel, double max);
	~Acceleration();
	void setAcceleration(double newAccel);
	double getAcceleration();
	
	void accelerate();

	void setSpeed(double speed);
	double getSpeed();
};