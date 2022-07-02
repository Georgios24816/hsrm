#pragma once

#include "CraftingTool.h"

class Saw: public CraftingTool
{
public:
	Saw();
	Saw(bool hasMotor, int initialNumberOfTeeth);

	virtual void operate(float seconds);
	void connectPlug(bool connection);

private:
	bool m_plugConnected = false;
	int m_initialNumberOfTeeth = 1;
	int m_numberOfTeeth = 1;
	int m_toothLoss = 500;
};
