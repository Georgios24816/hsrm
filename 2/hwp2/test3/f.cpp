class Car 
{
	int mileage;

	friend class CarManufacturer;
	friend void resetCarMileage(Car *);
};

class CarManufacturer 
{
	void resetMileage(Car &c) 
	{
		c.mileage = 0;
	}
};

void resetCarMileage(Car *c) {c->mileage = 0;}
