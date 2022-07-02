#pragma once

#include "Saw.h"

class ElectricJigsaw: public Saw
{
public:
	ElectricJigsaw();

	void operate(float seconds);
private:
	int m_movesPerMin = 1000;
};
