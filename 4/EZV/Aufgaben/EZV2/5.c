#include <wiringPi.h>
#include <stdint.h>

void handler()
{

}

int main()
{
    int pinSwitch2 = 27;
    int pin = 18;

    pinMode(pinSwitch2, INPUT);
    pinMode(pin, OUTPUT);
    

    return 0;
}