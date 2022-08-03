#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

void mySoftPwm(uint32_t us, uint8_t dutyCycle, int pin)
{
	assert(dutyCycle >= 0 && dutyCycle <= 100);
	softPwmCreate(pin, 0, 100);
	softPwmWrite(pin, dutyCycle);
	delayMicroseconds(us);
}

#define M_CounterMax 10

static unsigned int Counter = 0;
static unsigned int Micros1 = 0;
volatile unsigned int Micros2 = 0;
volatile bool ReadAvailable = 0;

void ISR()
{
	Counter++;
	ReadAvailable = 0;

	if (Counter == M_CounterMax)
	{
		Micros2 = micros() - Micros1;
		Counter = 0;
	} 

	else if (Counter == 1)
	{
		Micros1 = micros();
	}

	ReadAvailable = 1;
}

int main()
{
	int pin = 9;
	int softPwmPin = 8;

	assert(M_CounterMax % 2 == 0);

	wiringPiSetup();
	pinMode(pin, INPUT);
	pinMode(softPwmPin, OUTPUT);
	wiringPiISR(pin, INT_EDGE_RISING, &ISR);
	
	//pwmSetMode(PWM_MODE_MS);
	//pwmSetClock(8);
	//pwmSetRange(100);

	unsigned int oldMicros2 = 0;

	while (1)
	{
		if (ReadAvailable && Micros2 != oldMicros2)
		{
			oldMicros2 = Micros2;
int freq = 1 / ((int)(oldMicros2 / (M_CounterMax / 2)) * 0.000001);
			printf("-> %d\n", freq*60);
		}

		//mySoftPwm(10, 10, softPwmPin);
	}

	return 0;
}
