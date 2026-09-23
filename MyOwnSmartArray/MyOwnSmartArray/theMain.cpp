#include "cPerson.h"
#include <iostream>		// Console
#include <fstream>		// File IO
#include "cSmartArray.h"
//#include <vector>

int main()
{
	//std::vector<int> myVec;
	//for (unsigned int count = 0; count != 1000000000; count++)
	//{
	//	myVec.push_back(5728 + count);
	//}

	//while (!myVec.empty())
	//{
	//	myVec.erase(myVec.begin());
	//}


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
	cSmartArray myCensus;

//	unsigned int itemsToRead = 12400;
	unsigned int itemsToRead = 15;
	for (int index = 0; index != itemsToRead; index++)
	{
		cPerson tempPerson;
		dataFile >> tempPerson.Name;
		dataFile >> tempPerson.Gender;
		dataFile >> tempPerson.Population;

		// Like "push_back(tempPerson)"
		myCensus.AddItem(tempPerson);
	}
	for (int index = 0; index != itemsToRead; index++)
	{
		std::cout << myCensus.GetItemAtIndex(index).Name << std::endl;
	}


	return 0;
}