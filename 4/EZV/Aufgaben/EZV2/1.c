#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void mySoftPWM(uint32_t us, uint8_t dutyCycle, int pin)
{
    softPwmWrite
}

int main()
{
    int pin = 18;
    
    wiringPiSetup();
    softPwmCreate(pin, 0, 100); //wom range 0(off) - 100(on)


    while (1)
    {
        mySoftPWM(10000, 50, pin);
    }

    return 0;
}
