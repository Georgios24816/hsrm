#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>

bool hardwarePWM(int pin, size_t dutyCycle)
{
	if (dutyCycle > 100)
	{
		return false;
	}
	
	pwmWrite(pin, (dutyCycle * 1024) / 100);
	
	return true;
}

int main()
{
	wiringPiSetup();
	pinMode(1, PWM_OUTPUT);
	
	pwmWrite(1, 50);

	while (1);
    

    return 0;
}