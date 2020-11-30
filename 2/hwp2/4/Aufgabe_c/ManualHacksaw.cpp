#include "ManualHacksaw.h"

ManualHacksaw::ManualHacksaw(int numberOfHandles)
	: Saw(false, 100), m_numberOfHandles(numberOfHandles)
{

	Saw::CraftingTool::setNumberOfMoves(m_movesPerSec);
}

bool ManualHacksaw::isSingleHanded() const
{
	return m_numberOfHandles == 1;
}

void ManualHacksaw::operate(float seconds)
{
	std::cout << "Manual Hack";
	Saw::operate(seconds);
}
