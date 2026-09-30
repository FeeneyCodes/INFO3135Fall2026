#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO

#include "cMyLinkedList.h"

void doSTL(void);

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

//	cMyLinkedList myPeople;
	cMyLinkedList<cPerson> myPeople;
	//cMyLinkedList<double> myDoubles;
	//cMyLinkedList<std::string> myStrings;

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


	cMyLinkedList<double> myDoubles;
	myDoubles.InsertAtCurrent(33.4);
	myDoubles.InsertAtCurrent(12534.5);
	myDoubles.InsertAtCurrent(1.3);
	myDoubles.InsertAtCurrent(886.6);

	while (myDoubles.MovePrevious()) {};

	do
	{
		std::cout << myDoubles.GetAtCurrent() << std::endl;
	} while (myDoubles.MoveNext());

	
	cMyLinkedList<std::string> myStrings;
	myStrings.InsertAtCurrent("Hello");
	myStrings.InsertAtCurrent("HowAreYou");
	myStrings.InsertAtCurrent("Boo!");
	myStrings.InsertAtCurrent("Dogs and cats, living together!");

	while (myStrings.MovePrevious()) {};

	do
	{
		std::cout << myStrings.GetAtCurrent() << std::endl;
	} while (myStrings.MoveNext());



	return 0;
}