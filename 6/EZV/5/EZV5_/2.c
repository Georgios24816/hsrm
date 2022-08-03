#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int PinEnable = 21; //5
int PinIn1 = 6; //6
int PinIn2 = 26;//12

void motorDelay()
{
	delay(1000);
}

void motorSet(int enable, int in1, int in2)
{
	digitalWrite(PinEnable, enable);
	digitalWrite(PinIn1, in1);
	digitalWrite(PinIn2, in2);
}

void motorStepBackward()
{
	motorSet(HIGH, LOW, HIGH);
}

void motorStepForward()
{
	motorSet(HIGH, HIGH, LOW);
}

void motorFastStop()
{
	motorSet(HIGH, HIGH, HIGH);
}

void motorStop()
{
	motorSet(HIGH, LOW, LOW);
}

void motorZero()
{
	motorSet(LOW, LOW, LOW);
}

typedef enum MotorState{ MS_Backward, MS_Forward, MS_FastStop, MS_Stop, MS_Zero } MotorState_t;

void motorDo(MotorState_t ms)
{
	switch (ms)
	{
	case MS_Backward: motorStepBackward(); break;
	case MS_Forward: motorStepForward(); break;
	case MS_FastStop: motorFastStop(); break;
	case MS_Stop: motorStop(); break;
	case MS_Zero: motorZero(); break;
	}
}

void motorThread()
{
	while (1)
	{
		motorDo(MS_Forward);
		motorDelay();
		motorDo(MS_Backward);
		motorDelay();
	}
}

int cpuUsage()
{
	FILE* procFile = NULL;
	unsigned long long last_jiffies_work = 0;
	unsigned long long last_jiffies_idle = 0;
	// TODO: Beginn schleife
	procFile = fopen("/proc/stat", "r");
	if (procFile == NULL) {
		return 0;
	}
	unsigned long long jiffies_user, jiffies_nice, jiffies_system, jiffies_idle;
	int ret = fscanf(procFile, "%*s %llu %llu %llu %llu", &jiffies_user,
	&jiffies_nice, &jiffies_system, &jiffies_idle );
	// TODO: fehlerbehandlung
	fclose(procFile);
	unsigned long long jiffies_sum = jiffies_user + jiffies_nice + jiffies_system;
	printf("Gesamt auslastung: %llu \n",
	jiffies_sum *100 /(jiffies_sum+jiffies_idle));
	unsigned long long diff_jiffies_active = jiffies_sum - last_jiffies_work;
	unsigned long long diff_jiffies_idle = jiffies_idle - last_jiffies_idle;
	printf("Auslastung letzte Sekunde: %llu \n",
	diff_jiffies_active*100/(diff_jiffies_active + diff_jiffies_idle));
	last_jiffies_work = jiffies_sum;
	last_jiffies_idle = jiffies_idle;
	
	return (int)(jiffies_sum *100 /(jiffies_sum+jiffies_idle));
}

int main()
{
	wiringPiSetup();
	
	pinMode(PinEnable, OUTPUT);
	pinMode(PinIn1, OUTPUT);
	pinMode(PinIn2, OUTPUT);
	
	//motorThread();
	pinMode(21, OUTPUT);
	pinMode(23, OUTPUT);
	pinMode(24, OUTPUT);
	pinMode(25, OUTPUT);
	
	/*
	while (1)
	{
		digitalWrite(21, HIGH);
		digitalWrite(23, LOW);
		digitalWrite(24, HIGH);
		digitalWrite(25, HIGH);
		delay(100);
	}
	*/

	motorZero();

	while (1)
	{
		/*
		for (int i = 0; i < 5; i++)
		{
			printf("%d\n", i);
			motorDo(i);
			motorDelay();
		}*/

		for (int i = 0; i < 100; i++)
		{
		motorSet(1, 1, 0);
		delay(50);
		motorSet(1, 0, 1);
		delay(50);
		/*
		motorSet(0, 0, 0);
		delay(10);
		*/
		}

		motorZero();
		int a;
		printf("--> : ");
		scanf("%d", &a);
	}

	return 0;
}
