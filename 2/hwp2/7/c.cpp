struct Car
{
public:
	Car() : m_mileage(0) {}
	Car(int mileage) : m_mileage(mileage) {}

	int getMileage() const { return m_mileage; }

private:
	int m_mileage;
};
