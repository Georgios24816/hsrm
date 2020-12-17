#include <cassert>

class Person
{
public:
	enum class Zustand
	{
		Z_Ledig,
		Z_Verstorben,
		Z_Verheiratet,
		Z_Verwitwet,
		Z_Geschieden
	};
	
	enum class Geschlecht
	{
		Z_Mann,
		Z_Frau
	};
	
	void tod()
	{
		assert(m_z != Z_Verstorben);
		
		m_z = Z_Verstorben;
	}
	
	void heirat(Geschlecht g)
	{
		assert(m_z != Z_Verstorben && m_z != Z_Verheiratet);
		assert(m_g != g);
		
		m_z = Z_Verheiratet;
	}
	
	void todDesPartners()
	{
		assert(m_z == Z_Verheiratet);
		
		m_z = Z_Verwitwet;
	}
	
	void scheidung()
	{
		assert(m_z == Z_Verheiratet);
		
		m_z = Z_Geschieden;
	}
	
private:
	Zustand m_z;
	Geschlecht m_g;
};