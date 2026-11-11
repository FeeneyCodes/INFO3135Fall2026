#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "cBinaryTree.h"

// Get name and delete from vector
std::string getRandomName(std::vector<std::string>& vecCities)
{
	int randIndex = rand() % vecCities.size();
	std::vector<std::string>::iterator itCity = vecCities.begin();
	int curIndex = 0;
	for (; curIndex < randIndex; curIndex++) 
	{ 
		itCity++;
	}

	std::string cityFound = *itCity;
	vecCities.erase(itCity);
	return cityFound;		 
}

int main()
{
	// Load the CityList.txt file into a vector
	std::vector<std::string> vecCityNames;

	std::ifstream cityFile("CityList.txt");
	if (!cityFile.is_open())
	{
		std::cout << "Can't open file." << std::endl;
		return -1;
	}
	// Read the file into the vector
	std::string nextCity;
	while (cityFile >> nextCity)
	{
		vecCityNames.push_back(nextCity);
	}
	cityFile.close();

	// Now work with the tree
	cBinaryTree myBT;

	for (int count = 0; count != 25; count++)
	{
		std::string city = getRandomName(vecCityNames);
		myBT.AddData(city);
	}

	//for (int index = 0; index != vecCityNames.size(); index++)
	//{
	//	std::string city = vecCityNames[index];
	//	myBT.AddData(city);
	//}

	//myBT.AddData("Lucan");
	//myBT.AddData("Toronto");
	//myBT.AddData("Craven");

	std::cout << "Max depth: " << myBT.m_DeepestLevelAdded << std::endl;

//	myBT.PrintBreadth();
	myBT.PrintDepth();
	
	//sNode* pLondon = myBT.pFindNode("Perth");
	sNode* pLondon = myBT.pFindNode("London");

	std::cout << myBT.m_DeepestLevel << " depth" << std::endl;

// This will eventually be inside the Binary Tree class
	//myBT.pRoot = new sNode();
	//myBT.pRoot->data = city;

	//city = getRandomName(vecCityNames);
	//sNode* pNewNode = new sNode();
	//pNewNode->data = city;
	//if (city < myBT.pRoot->data)
	//{	// Add new node to the left
	//	myBT.pRoot->pLeft = pNewNode;
	//}
	//else
	//{   // Add new node to right
	//	myBT.pRoot->pRight = pNewNode;
	//}

	return 0;
}