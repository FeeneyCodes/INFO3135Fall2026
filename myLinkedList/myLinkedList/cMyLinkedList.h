#pragma once

#include "cPerson.h"

class cNode
{
public:
	cPerson thePerson;
	cNode* pNextNode = nullptr;		// 0
	cNode* pPriorNode = nullptr;	// 0
};

class cMyLinkedList
{
private:  
	cNode* pCurrentNode = nullptr;		// or 0 or NULL
	// 
	cNode* pHeadNode = nullptr;
	cNode* pTailNode = nullptr;
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