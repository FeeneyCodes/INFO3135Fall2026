#include "cSmartArray.h"

cSmartArray::cSmartArray()
{
	//  Allocate the array
	this->Resize();

}

// Allocate the larger array, copy, delete old one
void cSmartArray::Resize(void)
{
	// "Resize"
	this->size = this->size * 2;		// or size *= 2;
	// 1. Make a new, larger array

	cPerson* pNewBiggerArray = new cPerson[this->size];

	// 2. Copy the data from old to new
	for (unsigned int index = 0; index != this->indexNextItem; index++)
	{
		pNewBiggerArray[index] = this->pData[index];
	}

	// 3. Delete the old data
	delete [] this->pData;

	// 4. Point the pData pointer to this new array
	this->pData = pNewBiggerArray;


//	//cPerson* pData = nullptr
//	this->pData = new cPerson[this->size];
	return;
}

// push_back
void cSmartArray::AddItem(cPerson newPerson)
{
	// Is there space for another item??
	if (this->indexNextItem >= this->size)
	{
		// "Resize"
		// 1. Make a new, larger array
		// 2. Copy the data from old to new
		// 3. Point the pData pointer to this new array
		// 4. Delete the old data
		this->Resize();
	}


	// Add item at this location
	this->pData[this->indexNextItem] = newPerson;
	// Increment
	this->indexNextItem++;
	return;
}
// operator[]
cPerson cSmartArray::GetItemAtIndex(unsigned int index)
{
	return this->pData[index];
}


