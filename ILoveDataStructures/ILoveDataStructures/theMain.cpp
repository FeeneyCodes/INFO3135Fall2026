
// C: didn't have "by reference"
//		"pointer" 
// C++: Used one of the pointer symbols for "by reference"
//		& 

#include <iostream>	// cout << "hi";    cin >> a; 

int main()
{
	int a = 333; 
	int b = 273;

	// Special variables to hold memory
	// "&" in this case is "address of"
	std::cout << &a << std::endl;
	//std::cout << &b << std::endl;

	int* p_a = &a;		// "address of"
	std::cout << p_a << std::endl;

	// Dereferencing
	std::cout << *p_a << std::endl;
	// a = 61;
	*p_a = 61;	// That will change what "p_a" is pointing to
				// which is "a", so "a" = 61
	std::cout << "a = " << a << std::endl;
	
	p_a = &b;		// "address of"
	*p_a = 65;
	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;

	//a = 61;


	int c = a + b;

	std::cout << "c = " << c << std::endl;

	doThis(a);  // 62
	std::cout << a;

	int& b = a; 
	return 0;
}

void doThis(int c)
{
	c++;
	std::cout << c << std::endl;
}

void doThis(int &c)
{
	c++;
	std::cout << c << std::endl;
}

void doThis(int* p_c)
{
	*p_c++;	// (*p_c)++
	std::cout << *p_c << std::endl;
}