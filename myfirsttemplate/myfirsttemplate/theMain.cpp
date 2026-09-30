#include <string>

class cPerson
{
public:
	std::string Name;			// Lisa
	char Gender = '?';				// F
	unsigned int Population = 0;	// 52435
};

template <class T>
T addNumbers(T a, T b)
{
	T total = a + b;
	return total;
}


int main()
{
	cPerson Bob; cPerson Sally;
	cPerson f = addNumbers<cPerson>(Bob, Sally);

	float g = addNumbers(6.5f, 593.4f);
	float g1 = addNumbers(6, 593);


	return 0;
}


int addNumbers(int a, int b)
{
	int total = a + b;
	return total;
}
float addNumbers(float a, float b)
{
	float total = a + b;
	return total;
}
int addNumbers(float a, double b)
{
	int total = a + b;
	return total;
}

