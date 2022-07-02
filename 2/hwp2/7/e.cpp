class Bird
{
public:
	Bird() : m_weight(0) {}
	Bird(float initWeight) : m_weight(initWeight) {}
	Bird(float initWeight, int dummyInt) : m_weight(initWeight) {}

private:
	float m_weight;
};
