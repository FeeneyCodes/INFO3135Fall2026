#include "hashMap.h"

unsigned int cHashMap::m_calcHash(std::string value)
{
	// Add up the character values 
	// divide by the size of the array
	// "Michael"  
	unsigned int index = 0;
	for (int charIndex = 0; charIndex != value.size(); charIndex++)
	{
		index += (int)value[charIndex];
	}
	// do a modulus on the size of the array
	index = index % cHashMap::SIZEOFARRAY;
	return index;
}

cHashMap::cHashMap()
{
	// Allocate 
	theData.reserve(cHashMap::SIZEOFARRAY);
	for (int count = 0; count != cHashMap::SIZEOFARRAY; count++)
	{
		theData.push_back(cPerson());
	}
}

void cHashMap::Insert(std::string name, cPerson person)
{
	// Calculate the hash (index)
	unsigned int index = this->m_calcHash(person.Name);
	// Store at that index
	this->theData[index] = person;
}

bool cHashMap::GetAtIndex(std::string name, cPerson& personFound)
{
	// Calculate the hash (index)
	unsigned int index = this->m_calcHash(name);
	// Store at that index
	//this->theData[index] = person;
// HACK: This is wrong
	return true;
}



//bool GetAtIndex(std::string name, cPerson& personFound);