enum class UhrZustand
{
	UZ_StundenEinstellen, UZ_MinutenEinstellen, UZ_ZeitAnzeigen
};

class DigitalUhr
{
public:
	void set()
	{
		if (m_zustand == UhrZustand::UZ_StundenEinstellen)
			m_zustand = UhrZustand::UZ_MinutenEinstellen;
		
		else if (m_zustand == UhrZustand::UZ_MinutenEinstellen)
			m_zustand = UhrZustand::UZ_ZeitAnzeigen;
		
		else if (m_zustand == UhrZustand::UZ_StundenEinstellen)
			m_zustand = UhrZustand::UZ_StundenEinstellen;
		
		do();
	}
	
	void inc()
	{
		if (m_zustand == UhrZustand::UZ_StundenEinstellen)
			m_stunden = (m_stunden + 1) % 24;
		
		else if (m_zustand == UhrZustand::UZ_MinutenEinstellen)
			m_minuten = (m_minuten + 1) % 60;
		
		do();
	}

protected:
	void do() const
	{
		if (m_zustand == UhrZustand::UZ_StundenEinstellen)
			std::cout << "Stunden : " << m_stunden << std::endl;
		
		else if (m_zustand == UhrZustand::UZ_MinutenEinstellen)
			std::cout << "Minuten : " << m_minuten << std::endl;
		
		else if (m_zustand == UhrZustand::UZ_StundenEinstellen)
			std::cout << m_stunden << ":" << m_minuten << std::endl;
	}

private:
	UhrZustand m_zustand = UhrZustand::UZ_StundenEinstellen;
	uint8_t m_stunden = 0;
	uint8_t m_minuten = 0;
};