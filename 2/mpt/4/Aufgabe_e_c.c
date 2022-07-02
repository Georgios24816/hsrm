int main()
{
    volatile int* const out = (volatile int* const) (0x4000C000);
 	*out = (int)'A';

    return 0;
}
