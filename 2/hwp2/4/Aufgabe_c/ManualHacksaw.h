#pragma once

#include "Saw.h"

class ManualHacksaw: public Saw
{
public:
	ManualHacksaw(int numberOfHandles);
	
	bool isSingleHanded() const;

	void operate(float seconds);

private:
	int m_numberOfHandles = 1;
	int m_movesPerSec = 2;
};
