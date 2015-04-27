#pragma once
#include <string>

template <class T>
class Algorithm
{
	public:
		Algorithm(std::string name)
		{
			m_name = name;
		}
		~Algorithm(){}

		std::string getName()
		{
			return m_name;
		}

protected:
	std::string m_name;
};