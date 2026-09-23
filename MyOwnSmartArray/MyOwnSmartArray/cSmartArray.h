#pragma once

#include "cPerson.h"

class cSmartArray
{
public:
	cSmartArray();
	unsigned int size = 5;				// Current size
	// Index in the array of the NEXT item we are GOING to 
	//	insert (note: it's not there, yet)
	unsigned int indexNextItem = 0;	

	// pData "pointing to" zero (0)
	cPerson* pData = nullptr;	// = new cPerson[size];


	void AddItem(cPerson newPerson);		// push_back
	// x = vector[index], operator[]

	cPerson GetItemAtIndex(unsigned int index);  // op[]
//	bool GetItemAtIndex(unsigned int index, 
//		                cPerson& thePerson);  // op[]

	//cPerson& const operator[](size_t pos);
	void OverwriteItemAtIndex(unsigned int index, cPerson thePerson);


	// Allocate the larger array, copy, delete old one
	void Resize(void);

	// INsert at index (in the middle)
	void InsertAtIndex(cPerson newPerson, unsigned int index);
	// Insert at index #0
	// Random delete @ index
	// Shrink??? 
	void DestructiveResize(unsigned int newSize);
};

