#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO

#include "cMyStack.h"

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

	cMyStack myPeople;

	// Put stuff into the stack
	unsigned int itemsToLoad = 10;
	for (unsigned int count = 0; count != itemsToLoad; count++)
	{
		cPerson tempPerson;
		dataFile >> tempPerson.Name;
		dataFile >> tempPerson.Gender;
		dataFile >> tempPerson.Population;
		std::cout << "Pushing " << tempPerson.Name << " onto stack..." << std::endl;
		myPeople.Push(tempPerson);
	}

	std::cout << "\nReading from stack:" << std::endl;
	
	for (unsigned int count = 0; count != itemsToLoad; count++)
	{
		cPerson tempPerson = myPeople.Pop();
		std::cout << tempPerson.Name << std::endl;
	}


	return 0;
}