#include <wiringPi.h>
#include <unistd.h> //for sleep

int main()
{
    int pin = 21;

    wiringPiSetup();
    pinMode (pin, OUTPUT);

    while (1)
    {
        digitalWrite(pin, HIGH);
        sleep(1);
        digitalWrite(pin, LOW);
        sleep(1);
    }

    return 0;
}