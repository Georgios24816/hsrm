#include <softPwm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

volatile size_t TachoPulses = 0;
static unsigned long Target;
static uint8_t DutyCycle;

void ISR(void)
{
    TachoPulses++;
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
        usleep(100000);

        printf("\rRPM = %d", (int)(TachoPulses / 30));
        TachoPulses = 0;

        fflush(stdout);
    }


    return 0;
}