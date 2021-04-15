#include <wiringPi.h>
#include <stdio.h>

int main()
{
    int pin = 21; //SW2 and later 20
    pinMode(pin, INPUT);

    while (1)
    {
        if (digitalRead(pin) == HIGH)
        {
            printf("switch pressed!");
        }
    }

    return 0;
}