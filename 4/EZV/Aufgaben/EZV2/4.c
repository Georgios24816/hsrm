#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h> //strtoul

int main(int argc, char* argv[])
{
    int pin = 2; //SW2
    size_t us = 100000;

    pinMode(pin, INPUT);

    if (argc == 2)
    {
        us = strtoul(argv[1], NULL, 0);
    }

    while (1)
    {
        scanf("%zu", &us);
        delayMicroseconds(us);
    }
    

    return 0;
}
