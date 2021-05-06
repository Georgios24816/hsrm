#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h> //strtoul

int main(int argc, char* argv[])
{
    int switchPin = 2; //SW2
	int gpioPin = 18; //GPIO 18
	
	wiringPiSetup();
    pinMode(switchPin, INPUT);
	pinMode(gpioPin, OUTPUT);
	
    while (1)
    {
		if (digitalRead(switchPin) == HIGH)
		{
			digitalWrite(gpioPin, HIGH);
			delayMicroseconds(1);
			digitalWrite(gpioPin, LOW);
		}
    }
    

    return 0;
}
