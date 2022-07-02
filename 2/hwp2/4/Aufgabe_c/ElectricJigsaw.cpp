#include "ElectricJigsaw.h"

ElectricJigsaw::ElectricJigsaw()
	: Saw(true, 1000)
{
	Saw::CraftingTool::setNumberOfMoves(m_movesPerMin / 60);
}

void ElectricJigsaw::operate(float seconds)
{
	std::cout << "Electric ";
	Saw::operate(seconds);
}

