
// C: didn't have "by reference"
//		"pointer" 
// C++: Used one of the pointer symbols for "by reference"
//		& 

#include <iostream>	// cout << "hi";    cin >> a; 

class cPerson
{
	int age;
	float height;
};

int main(int argc, char* argv[])
{
	cPerson Bob;					// STACK
	cPerson* pBob = new cPerson();	// HEAP


	//int t1 = 3;			// 1002
	//int t2 = 5;			// 1003
	//int t3 = 71;			// 1004
	////....
	//int t100 = 92;

	//int tArray[100];

	int tArray[5] = { 8, 27, 288, 234, 17 };
	int sizeoftArray = 5;
	// tArray[0] = 1
	// tArray[1] = 27

	for (int index = 0; index < 5; index++)
	{
		std::cout << index << " : " << tArray[index] << std::endl;
	}
	std::cout << "----------------" << std::endl;

	// Pointing to the start of the array
	int* pArray = tArray;	// int* pArray = &( tArray[0] );
	
	//int theVal = tArray[1];
	int* pArray2 = &(tArray[0]);
	std::cout << *pArray2 << std::endl;

	// int tArray[5] = { 8, 27, 288, 234, 17 };
	std::cout << "--------------------" << std::endl;
	std::cout << " fun with pointers!! " << std::endl;
	int* pArrayLoc = tArray;	//  of int* pArrayLoc = &(tArray[0])
	for (int index = 0; index < 10; index++)
	{
		std::cout << *pArrayLoc << std::endl;
		pArrayLoc++;	// Move to next array location
	}
	std::cout << "--------------------" << std::endl;

	//int* t; 
	std::cout << tArray[0] << std::endl;
	std::cout << tArray << std::endl;
	std::cout << &( tArray[0] ) << std::endl;
	std::cout << sizeof(tArray) << std::endl;
	std::cout << "----------------" << std::endl;




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

//	doThis(a);  // 62
	std::cout << a;

//	int& b = a; 
	return 0;
}

//void doThis(int c)
//{
//	c++;
//	std::cout << c << std::endl;
//}
//
//void doThis(int &c)
//{
//	c++;
//	std::cout << c << std::endl;
//}
//
//void doThis(int* p_c)
//{
//	*p_c++;	// (*p_c)++
//	std::cout << *p_c << std::endl;
//}