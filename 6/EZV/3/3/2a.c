#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define M_Pause 10000

int main(int argc, char* argv[])
{
	const unsigned int printPeriodMs = 5000;
	int pinGpio = 28;
	bool oldState = false;
	bool newState = false;
	bool started = false;
	float frequency = 0;
	size_t pulseCounter = 0;
	unsigned int millisCounter;
	unsigned int oldMicros, newMicros;
	size_t microsCounter = 0;

	wiringPiSetup();
	
	pinMode(pinGpio, OUTPUT);
	
	while (digitalRead(pinGpio) == oldState);
	while (digitalRead(pinGpio) != oldState);

	millisCounter = millis();
	while (1)
	{
		newState = digitalRead(pinGpio);

		if (newState != oldState)
		{
			unsigned int currentMicros = micros();
			oldState = newState;

			if (!started)
			{
				oldMicros = currentMicros;
				started = true;
			}

			else
			{
				unsigned int currentMillis = millis();
				newMicros = currentMicros;
				started = false;

				//frequency += (float)1 / (float)((newMicros - oldMicros) * 0.000002);

				microsCounter += (newMicros - oldMicros);
				pulseCounter++;

				if (currentMillis - millisCounter >= printPeriodMs)
				{
			frequency = 1.0f / (float)(0.000002f * microsCounter / pulseCounter);
			printf("--> %f Hz\n", (float)(frequency / 1/*pulseCounter*/));
					millisCounter = currentMillis;
					//frequency = 0;
					microsCounter = 0;
					pulseCounter = 0;
				}
			}

		}
#if M_Pause
	delayMicroseconds(M_Pause);
#endif
	}

	return 0;
}
