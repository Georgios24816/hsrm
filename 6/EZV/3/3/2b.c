#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const unsigned int printPeriodMs = 5000;
static bool oldState = false;
static bool newState = false;
static bool started = false;
static float frequency = 0;
static size_t pulseCounter = 0;
static unsigned int millisCounter;
static unsigned int oldMicros, newMicros;
static size_t microsCounter = 0;

static volatile bool PrintFreq = false;

void ISR(void)
{
	unsigned int currentMicros = micros();

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
	//printf("--> %f Hz\n", (float)(frequency / 1/*pulseCounter*/));
			millisCounter = currentMillis;
			//frequency = 0;
			PrintFreq = true;
			microsCounter = 0;
			pulseCounter = 0;
		}
	}

}

int main(int argc, char* argv[])
{
	int pinGpio = 28;

	wiringPiSetup();
	
	pinMode(pinGpio, OUTPUT);
	
	wiringPiISR(pinGpio, INT_EDGE_BOTH, &ISR);

	millisCounter = millis();

	while (1)
	{
		if (PrintFreq)
		{
			printf("--> %f Hz\n", (float)(frequency / 1/*pulseCounter*/));
			PrintFreq = false;
		}
	}
	/*
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
			printf("--> %f Hz\n", (float)(frequency / 1pulseCounter));
					millisCounter = currentMillis;
					//frequency = 0;
					microsCounter = 0;
					pulseCounter = 0;
				}
			}

		}

	}
*/

	return 0;
}
