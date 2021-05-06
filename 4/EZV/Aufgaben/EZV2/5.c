#include <wiringPi.h>
#include <stdint.h>

static int pin = 18;

void ISRhandler()
{
	digitalWrite(pin, HIGH);
	delayMicroseconds(1); //??
	digitalWrite(pin, LOW);
}

int main()
{
    int pinSwitch2 = 27;

	wiringPiSetup();
    pinMode(pinSwitch2, INPUT);
    pinMode(pin, OUTPUT);

	wiringPiISR(pinSwitch2, INT_EDGE_RISING, ISRhandler);
	
	while (1);

    return 0;
}