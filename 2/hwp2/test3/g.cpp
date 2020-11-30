class Car
{
public:

private:
	Car() {}
	
	friend Car* createCar();
};

Car* createCar()
{
	return new Car();
}
