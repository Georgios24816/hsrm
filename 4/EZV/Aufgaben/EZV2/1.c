#include <wiringPi.h>
#include <stdint.h>

void mySoftPWM(uint32_t us, uint8_t dutyCycle, int pin)
{
    delayMicroseconds(us);
}

int main()
{
    int pin = 18;

    while (1)
    {
        mySoftPWM(10000, 50, pin);
    }

    return 0;
}