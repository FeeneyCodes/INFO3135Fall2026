#pragma once
#include "cPerson.h"
#include <string>
#include <vector>

class cHashMap
{
public:
	cHashMap();
	// std::map<std::string, cPerson> mapPeople;
	// mapPeople[tempPerson.Name] = tempPerson;
	void Insert(std::string name, cPerson person);
	// cout << mapPeople["Mary"]
	bool GetAtIndex(std::string name, cPerson& personFound);

//private:
	// Given a key, generates a number
	unsigned int m_calcHash(std::string value);

	static const unsigned int SIZEOFARRAY = 100;
//	cPerson theData[SIZEOFARRAY];
	std::vector<cPerson> theData;
};