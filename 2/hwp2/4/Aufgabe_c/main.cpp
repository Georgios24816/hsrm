#include "ElectricJigsaw.h"
#include "ManualHacksaw.h"

int main()
{
	ElectricJigsaw s1;
	s1.operate(1);

	ManualHacksaw m1(1);
	m1.operate(1);

	return 0;
}
