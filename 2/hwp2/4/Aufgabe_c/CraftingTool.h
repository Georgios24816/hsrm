#pragma once

#include <iostream>

class CraftingTool
{
public:
	CraftingTool(bool hasMotor);
	
	virtual void operate(float seconds);

	int getNumberOfMoves() const;
	void setNumberOfMoves(int moves);
	void addNumberOfMoves(int moves);

	bool getHasMotor() const;
	void setHasMotor(bool hasMotor);

	float getMovesPerSecond() const;
	void setMovesPerSecond(float moves);

protected:
	bool m_hasMotor = false;
	float m_movesPerSecond = 1;

private:
	int m_numberOfMoves = 0;
};
