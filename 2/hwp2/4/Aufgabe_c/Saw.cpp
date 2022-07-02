#include "Saw.h"

Saw::Saw()
	: CraftingTool(false)
{

}

Saw::Saw(bool hasMotor, int initialNumberOfTeeth)
	: CraftingTool(hasMotor), m_initialNumberOfTeeth(initialNumberOfTeeth),
	  m_numberOfTeeth(initialNumberOfTeeth)
{

}

void Saw::operate(float seconds)
{
	int movesNr = getNumberOfMoves();
	int newMoves = seconds * getMovesPerSecond();
	int teethNeeded = (movesNr + newMoves) / m_toothLoss;

	if (m_initialNumberOfTeeth >= teethNeeded)
	{
		addNumberOfMoves(getMovesPerSecond() * seconds);
		m_numberOfTeeth = m_initialNumberOfTeeth - teethNeeded;
	}

	else
		std::cout << "not enough teeth to use the ";
	
	std::cout << "SAW\n";
}

void Saw::connectPlug(bool connection)
{
	if (getHasMotor())
	   m_plugConnected = connection;	
}
