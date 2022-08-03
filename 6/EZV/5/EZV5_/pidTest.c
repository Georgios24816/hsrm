#include "pid.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define M_CounterMax 10

static unsigned int Counter = 0;
static unsigned int Micros1 = 0;
volatile unsigned int Micros2 = 0;
volatile bool ReadAvailable = 0;

int pwmInput = 10;
int pwmInput2 = 10;
double y = 0;
int pin = 1;

typedef struct CmdArgs
{
	unsigned int cycleTime;
	unsigned int target;
	float p;
	float i;
	float d;
} CmdArgs_t;

volatile CmdArgs_t Config;

void ISR()
{
	if (Counter > M_CounterMax) return;

	Counter++;

	if (Counter == M_CounterMax)
	{
		Counter++;
		Micros2 = micros() - Micros1;
		
int freq = (float)(1 / ((Micros2 / (M_CounterMax / 2)) * 0.000001)) * 60;
			y = y + output(freq);
			//printf("%d | %d\n", freq, pwmInput2);
			if (freq <= Config.target)
			{
				putchar('<');
			}
			else
			{
				putchar('>');
			}

			printf("%d\n", freq);

			//pwmInput -= y / Config.target;
			if (y < 0) pwmInput2 += 10;
			else pwmInput2 -= 10;

			if (pwmInput2 >= 100) pwmInput2 = 99;
			if (pwmInput2 <= 3) pwmInput2 = 4;

			pwmWrite(pin, pwmInput2);
		
		Counter = 0;
	} 

	else if (Counter == 1)
	{
		Micros1 = micros();
		ReadAvailable = 1;
	}
}

CmdArgs_t parseArgs(int argc, char* argv[])
{
	if (argc < 3)
	{
		assert(false);
	}

	CmdArgs_t ret;

	int index = 1;

	if (index + 2 <= argc && strcmp(argv[index], "-c") == 0)
	{
		ret.cycleTime = atoi(argv[++index]);
		index++;
	}

	else if (index + 2 <= argc && strcmp(argv[index], "-s") == 0)
	{
		ret.target = atoi(argv[++index]);
		index++;
	}

	else if (index + 2 <= argc && strcmp(argv[index], "-p") == 0)
	{
		ret.p = atof(argv[++index]);
		index++;
	}
	
	else if (index + 2 <= argc && strcmp(argv[index], "-i") == 0)
	{
		ret.i = atoi(argv[++index]);
		index++;
	}
	
	else if (index + 2 <= argc && strcmp(argv[index], "-d") == 0)
	{
		ret.d = atoi(argv[++index]);
		index++;
	}

	return ret;
}

void configureArgs()
{
	char c;
	
	switch (c)
	{
		case 'S': Config.target++; break;
		case 's': Config.target--; break;
		case 'P': Config.p++; break;
		case 'p': Config.p--; break;
		case 'I': Config.i++; break;
		case 'i': Config.i--; break;
		case 'D': Config.d++; break;
		case 'd': Config.d--; break;
	};
}

int main(int argc, char** argv)
{
	//assert(M_CounterMax % 2 == 0);
	//Config = parseArgs(argc, argv);
	Config.p = 1.1;
	Config.i = 1;
	Config.d = 0.001;
	Config.target = 2350;

	
	int val = 0;
	//double y = 0;
	int pwmInput = 10;
	//int pwmInput2 = 10;

	int inpPin = 9;
	wiringPiSetup();
	pinMode(pin, PWM_OUTPUT);
	pinMode(inpPin, INPUT);
	wiringPiISR(inpPin, INT_EDGE_RISING, &ISR);
	
	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(8);
	pwmSetRange(100);
	pwmWrite(pin, pwmInput2);
    
	initPid(Config.p, Config.i, Config.d);

	/*
	while (1)
	{
		for (int i = 0; i < 10; i++)
		{
			y = y + output(y);
			printf("y : %lf\n", y);
			delay(250);
		}
	
		if (val > 0) 	val = 0;
		else 		val = 5;
	}
	*/

	unsigned int oldMicros2 = 0;

	//const int TargetFreq = 1800;
	setSetPoint(Config.target);
	while (1)
	{
		if (1) //ReadAvailable && Micros2 != oldMicros2)
		{
			/*
			oldMicros2 = Micros2;
int freq = (float)(1 / ((oldMicros2 / (M_CounterMax / 2)) * 0.000001)) * 60;
			y = y + output(freq);
			printf("%d | %d | %d\n", freq, (int)y, pwmInput);

			pwmInput -= y / Config.target;
			if (y < 0) pwmInput2++;
			else pwmInput2--;

			if (pwmInput2 >= 100) pwmInput2 = 99;
			if (pwmInput2 <= 3) pwmInput2 = 4;

			pwmWrite(pin, pwmInput2);
			*/
			/*
			if (freq > TargetFreq*1.01)
			{
				//if (pwmInput > 11)
				//{
					//pwmInput -= 9;
					pwmInput = 10;
					pwmWrite(pin, pwmInput);
				//}
			}
			else if (freq < TargetFreq*0.99)
			{
				//if (pwmInput < 91)
				//{
					//pwmInput += 9;
					pwmInput = 100;
					pwmWrite(pin, pwmInput);
				//}
			}
			if (freq > 2000)
				pwmWrite(pin, 30);

			else
				pwmWrite(pin, 70);
			*/
		}
	}

	return 0;
}
