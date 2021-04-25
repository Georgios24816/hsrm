#include <wiringPi.h>

int main()
{
    int pin = 29;
    pinMode(pin, OUTPUT);

    while (1)
    {
        digitalWrite(pin, HIGH);
        digitalWrite(pin, LOW);
    }

    return 0;
}
