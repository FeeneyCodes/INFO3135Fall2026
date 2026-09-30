#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO

#include "cMyLinkedList.h"

void doSTL(void);

int main()
{

	int a = 43;
	int b = 67;
	int c = a + b;
	std::cout << "total = " << c << std::endl;



	doSTL();

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

	cMyLinkedList myPeople;

	// Put stuff into the stack
	unsigned int itemsToLoad = 10;
	for (unsigned int count = 0; count != itemsToLoad; count++)
	{
		cPerson tempPerson;
		dataFile >> tempPerson.Name;
		dataFile >> tempPerson.Gender;
		dataFile >> tempPerson.Population;
		std::cout << "Inserting " << tempPerson.Name << " onto stack..." << std::endl;
		
		myPeople.InsertAtCurrent(tempPerson);
	}

	std::cout << "\nReading from linked list:" << std::endl;
	
	// move to the start of the list
	while (myPeople.MovePrevious())
	{
		// Loops until MovePrevious returns false
	}

	// move current to "Karen"
	for (unsigned int count = 0; count != 5; count++)
	{
		cPerson tempPerson = myPeople.GetAtCurrent();

		std::cout << tempPerson.Name << std::endl;

		myPeople.MoveNext();
	}

	// Remove the data at the current node
	// (we are pointing to "Karen" as the current node)
	myPeople.DeleteAtCurrent();

	std::cout << "---------------------" << std::endl;

	// Move back to the head again
	while (myPeople.MovePrevious())
	{
		// Loops until MovePrevious returns false
	}

	//for (unsigned int count = 0; count != 10; count++)
	do
	{
		cPerson tempPerson = myPeople.GetAtCurrent();

		std::cout << tempPerson.Name << std::endl;
	}
	while (myPeople.MoveNext());



	//cPerson Bob;		// STACK
	//Bob.Name = "Bob";

	//cPerson* pSally = new cPerson();
	//pSally->Name = "Sally";


	return 0;
}