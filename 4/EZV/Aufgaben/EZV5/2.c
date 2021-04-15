#include <wiringpi.h>

size_t fanF(size_t pulses)
{
	return pulses / 2;
}

size_t fanRPM(size_t pulseIntervalUS)
{
	return pulseIntervalUS;
}

int main(int argc, char* argv[])
{
	int pulsePin = 3;//
	int softwarePWMpin = 2;//
	int hardwarePWMpin = 18;//
	
	size_t avgF = 25000;
	size_t minF = 21000;
	size_t maxF = 28000;
	

	return 0;
}