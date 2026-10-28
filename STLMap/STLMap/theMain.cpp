#include "cPerson.h"
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iostream>

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
	//	Lisa    F    52435
	//	Kimberly    F    33100
	//	Michelle    F    30824

	std::map<int, std::string> mapArray;

	std::string nextName;

	while (dataFile >> nextName)
	{
		std::string gender;
		int numberOfBabies = 0;
		dataFile >> gender >> numberOfBabies;
		mapArray[numberOfBabies] = nextName;
	}

	std::cout << mapArray[5] << std::endl;
//	mapArray[5] = "Ted";
//	std::cout << mapArray[5] << std::endl;

	std::map<int, std::string>::iterator itZev = mapArray.find(5);
	if (itZev != mapArray.end())
	{
		// Found it!
		mapArray.erase(itZev); 
	}

	for (std::map<int, std::string>::reverse_iterator itMap = mapArray.rbegin();
		itMap != mapArray.rend(); itMap++)
	{
		std::cout << itMap->first << ":" << itMap->second << std::endl;
	}

	std::set<std::string> setNames;
	//std::string nextName;

	//while (dataFile >> nextName)
	//{
	//	setNames.insert(nextName);
	//	std::string discard;
	//	dataFile >> discard >> discard;
	//}


	return 0;
}