#pragma once

#include "cPerson.h"

class cNode
{
public:
	cPerson thePerson;
	cNode* pNextNode = nullptr;
	cNode* pPriorNode = nullptr;
};

class cMyLinkedList
{
private:  
	cNode* pCurrentNode = nullptr;		// or 0 or NULL
public:
	void InsertAtCurrent(cPerson newPerson);	// **
	cPerson GetAtCurrent(void);					// **
	bool DeleteAtCurrent(void);

	// Returns true if it DID move
	bool MoveNext(void);						// **
	bool MovePrevious(void);					// **

	void MoveToHead(void);	// Or "start"
	void MoveToTail(void);	// Or "end"

};