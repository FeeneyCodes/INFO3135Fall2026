
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
	unsigned int arraySize = 12400;
	//	const unsigned int ARRAYSIZE = 1000000;
		// Compile time array on the STACK
		// (treated like a pointer, because it IS a pointer)

//	cPerson peopleArray[ARRAYSIZE];		// STACK
	// Note: "*" and "new"
	cPerson* pPeopleArray = new cPerson[arraySize];	// HEAP
	
//	int x;
//	int* pX = new int();		// Make 1 thing
//	int* pXA = new int[100];	// Make 100 things

	//int x[10];

	for (int index = 0; index != arraySize; index++)
	{
		//std::cin >> tempPerson.Name;
		//std::cin >> tempPerson.Gender;
		//std::cin >> tempPerson.Population;
		dataFile >> pPeopleArray[index].Name;
		dataFile >> pPeopleArray[index].Gender;
		dataFile >> pPeopleArray[index].Population;
	}

	std::cout << pPeopleArray[arraySize-1].Name << std::endl;

	// What if? 
	// - we don't know how big to make the array?
	// - we want to add one more name?







	// **********************************************
	// Double the size of the array
	// 
	// unsigned int arraySize = 12400;
//	cPerson* pPeopleArray = new cPerson[arraySize];	// HEAP

	// NEW array
	unsigned int newSize = arraySize * 2;
	cPerson* pBiggerArray = new cPerson[newSize];

	// Copy the data from the old to the new
	for (int index = 0; index != arraySize; index++)
	{
		pBiggerArray[index] = pPeopleArray[index];
	}

	cPerson* pOldArrayToDelete = pPeopleArray;

	// Pointing the old pointer to the new array
	pPeopleArray = pBiggerArray;

	// Get rid of OG smaller array
	delete[] pOldArrayToDelete;

	std::cout << pPeopleArray[arraySize - 1].Name << std::endl;

	return 0;
}

