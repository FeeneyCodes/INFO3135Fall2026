#pragma once

#include "cPerson.h"

class cSmartArray
{
public:
	cSmartArray();
	unsigned int size = 5;				// Current size
	unsigned int indexNextItem = 0;

	cPerson* pData = nullptr;	// = new cPerson[size];

	void AddItem(cPerson newPerson);		// push_back
	cPerson GetItemAtIndex(unsigned int index);  // op[]

	// Allocate the larger array, copy, delete old one
	void Resize(void);

	// INsert at index (in the middle)
	// Insert at index #0
	// Random delete @ index
	// Shrink??? 
};

