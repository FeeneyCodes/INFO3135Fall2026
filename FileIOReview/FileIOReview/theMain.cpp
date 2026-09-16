
#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO


int main(void)
{
	//Lisa    F    52435
	//Kimberly    F    33100
	//Michelle    F    30824

	// Open a file for reading
	std::ifstream dataFile("yob1967_no_commas.txt");

	if (!dataFile.is_open())
	{
		std::cout << "Didn't open file!" << std::endl;
		return -1;	// Exit with error
	}
	else
	{
		std::cout << "File is open!" << std::endl;
	}

	// Array
	const unsigned int ARRAYSIZE = 12400;
//	const unsigned int ARRAYSIZE = 1000000;
	// Compile time array on the STACK
	// (treated like a pointer, because it IS a pointer)
	cPerson peopleArray[ARRAYSIZE];		




	//int x[10];

	for (int index = 0; index != ARRAYSIZE; index++)
	{
		//std::cin >> tempPerson.Name;
		//std::cin >> tempPerson.Gender;
		//std::cin >> tempPerson.Population;
		dataFile >> peopleArray[index].Name;
		dataFile >> peopleArray[index].Gender;
		dataFile >> peopleArray[index].Population;
	}

	std::cout << peopleArray[ARRAYSIZE-1].Name << std::endl;

	return 0;
}
