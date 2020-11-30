#include <iostream>

class Animal {
public:
virtual void move() { std::cout << "ANIMAL MOVE\n"; }
void sleep() {	std::cout << "ANIMAL SLEEP\n"; }
};

class Bird : private Animal {
public:
virtual void move() { std::cout << "BIRD MOVE\n";}
void sleep() { std::cout << "BIRD SLEEP\n"; }
};

class Bee : public Animal {
public:
virtual void move() { std::cout << "BEE MOVE\n";}
void sleep() { std::cout << "BEE SLEEP\n"; }
};

class Cat : public Animal {
public:
void move() { std::cout << "CAT MOVE\n";}
void sleep() { std::cout << "CAT SLEEP\n";}
};

int main(void) {
	Bird bird; Bee bee; Cat cat;
	Animal *pAnimalCat = &cat;
	//Animal *pAnimalBird = &bird;
	Animal &rAnimalBee = bee;
	bee.move();
	pAnimalCat->move();
	pAnimalCat->sleep();
	rAnimalBee.move();
	//pAnimalBird->move();
	
	return 0;
}
