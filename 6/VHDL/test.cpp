#include <iostream>
#include <cstdint>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) (			\
	a += b,  d ^= a,  d = ROTL(d,16),	\
	c += d,  b ^= c,  b = ROTL(b,12),	\
	a += b,  d ^= a,  d = ROTL(d, 8),	\
	c += d,  b ^= c,  b = ROTL(b, 7))
#define ROUNDS 20

int main()
{
	uint32_t a = 1;
	uint32_t b = 0;
	uint32_t c = 0;
	uint32_t d = 0;

	std::cout << std::hex << a << " " << b << 
		" " << c << " " << d << std::endl;
	QR(a, b, c, d);
	std::cout << std::hex << a << " " << b << 
		" " << c << " " << d << std::endl;
	
	return 0;
}
