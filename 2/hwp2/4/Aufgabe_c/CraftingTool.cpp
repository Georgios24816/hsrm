#include "CraftingTool.h"

CraftingTool::CraftingTool(bool hasMotor)
	: m_hasMotor(hasMotor)
{

}

void CraftingTool::operate(float seconds)
{
	m_numberOfMoves += m_movesPerSecond * seconds;
	std::cout << "CraftingTool\n";
}

int CraftingTool::getNumberOfMoves() const
{
	return m_numberOfMoves;
}

void CraftingTool::setNumberOfMoves(int moves)
{
	m_numberOfMoves = moves;
}

void CraftingTool::addNumberOfMoves(int moves)
{
	m_numberOfMoves += moves;
}

bool CraftingTool::getHasMotor() const
{
	return m_hasMotor;
}

void CraftingTool::setHasMotor(bool hasMotor)
{
	m_hasMotor = hasMotor;
}

float CraftingTool::getMovesPerSecond() const
{
	return m_movesPerSecond;
}

void CraftingTool::setMovesPerSecond(float moves)
{
	m_movesPerSecond = moves;
}
