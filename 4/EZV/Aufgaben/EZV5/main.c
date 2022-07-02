#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#define M_NUMPERIODS 20
#define M_CYCLETIME 1000000

volatile size_t TachoPulses = 0;
unsigned long Target;
uint8_t DutyCycle;
size_t Rpm;

unsigned int t1 = 0, t2 = 0;
unsigned int periodSum = 0;

void ISR(void)
{
    if (digitalRead(3) != HIGH)
    {
        return;
    }

    t2 = micros();
    unsigned int period = t2 - t1;

    TachoPulses++;
    t1 = t2;
    TachoPulses++;

    periodSum += period;

    if (TachoPulses > M_NUMPERIODS)
    {
        Rpm = (30000000 * TachoPulses) / periodSum;
        periodSum = tacho = 0;
    }
}

int main(int argc, char** argv)
{
    int cmd;
    size_t delayMs = 50;

    while ( (cmd = getopt(argc, argv, "?d:h")) != -1 )
    {
        if (cmd == 'r')
        {
            char* end;
            Target = strtoul(optarg, &end, 0);

            if (optarg == end)
            {
                printf("NAN\n");
                exit(EXIT_FAILURE);
            }

            if (DutyCycle > 100)
            {
                printf("Invalid duty-cycle\n");
                exit(EXIT_FAILURE);
            }

        }

        else
        {
            exit(EXIT_FAILURE);
        }
    }

    wiringPiSetupGpio();
    t1 = micros();

    wiringPiISR(3, INT_EDGE_RISING, ISR);
    pinMode(18, PWM_OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(8);
    pwmSetRange(100);
    pwmWrite(18, DutyCycle);

    softPwmCreate(2, 0, 100);
    softPwmWrite(2, DutyCycle);

    while (1)
    {
        usleep(M_CYCLETIME);
        printf("\rRPM = %d", (int)Rpm);
        fflush(stdout);
    }


    return 0;
}