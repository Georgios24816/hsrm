class CookingTimer
{
public:
	CookingTimer(int seconds) : m_seconds(seconds), m_secondsRemaining(seconds) {}

	int getSeconds() const { return m_seconds; }
	void setSeconds(int seconds) 
	{ 
		m_seconds = seconds; 
		m_secondsRemaining = seconds;
	}

	int getSecondsRemaining() const { return m_secondsRemaining; }
	void setSecondsRemaining(int seconds)
	{
		//Implementierung nicht verlangt
		if (seconds <= m_seconds)
			m_secondsRemaining = seconds;
	}


	void startCountdown() {}
	void stopCountdown() {}
	void printCountdown() {}
	void reset() { m_secondsRemaining = m_seconds; }

private:
	int m_seconds;
	int m_secondsRemaining;
};

