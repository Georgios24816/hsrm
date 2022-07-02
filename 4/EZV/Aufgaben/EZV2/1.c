#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void mySoftPWM(uint32_t us, uint8_t dutyCycle, int pin)
{
    softPwmCreate(pin, 0, 100); //pwm range 0(off) - 100(on)
    softPwmWrite(pin, dutyCycle);
    delayMicroseconds(us);
}

int main()
{
    int pin = 1;
    
    wiringPiSetup();

    while (1)
    {
        mySoftPWM(1000, 10, pin);
    }

    return 0;
}
