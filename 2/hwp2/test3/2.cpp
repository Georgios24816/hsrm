class Food 
{
protected:
int weight;
};

class Vegetable 
{
};

class Soup 
{
};

class VegetableSoup : public Vegetable, public Soup, public Food
{
	bool checkForUniqueWeight()
	{
		Food::weight
		return true;
	}

};
