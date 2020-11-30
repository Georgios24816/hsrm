class Runway
{
public:
	Runway() : m_direction(0) {}
	Runway(int dir) : m_direction(dir) {}
	~Runway() {}

	Runway(const Runway& runway) : m_direction(runway.m_direction) {}
	Runway& operator = (const Runway& runway) 
	{
		if (this == &runway)
			return *this;

		m_direction = runway.m_direction;

		return *this;
	}

	static float getAirTemperature() { return m_airTemperature; }

private:
	int m_direction;
	static float m_airTemperature;
};

float Runway::m_airTemperature = 20;

//Aufgabe h
int main()
{
	Runway r(10);

	r.getAirTemperature(); //1
	Runway::getAirTemperature(); //2
}
