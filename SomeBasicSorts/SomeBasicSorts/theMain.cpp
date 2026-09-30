#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO
#include <vector>

int main()
{


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

	// Container holding the data
	std::vector<cPerson> myPeople;

	unsigned int itemsToRead = 10;
	for (int index = 0; index != itemsToRead; index++)
	{
		cPerson tempPerson;
		dataFile >> tempPerson.Name;
		dataFile >> tempPerson.Gender;
		dataFile >> tempPerson.Population;

		// Like "push_back(tempPerson)"
		myPeople.push_back(tempPerson);
	}

	// 10 people are read





	for (int index = 0; index != itemsToRead; index++)
	{
		std::cout << myPeople[index].Name << std::endl;
	}


	return 0;
}