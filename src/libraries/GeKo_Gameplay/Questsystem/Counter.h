#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

/**A class which provides a counter which can be used as a time trigger.*/
class Counter 
{
public: 
	Counter();
	Counter(float time);
	~Counter();

	///The time which has to be passed will be set
	void setTime(float time);
	float getTime();

	///The counter will be started
	void start();

	///The counter will be ended
	void end();

	///The counter will be updated per second (60 frames!)
	void update();

protected:
	float m_time;
	bool m_started;
	bool m_finished;
	int m_frame;
};