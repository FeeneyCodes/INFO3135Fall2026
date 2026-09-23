#include "cMyLinkedList.h"


void cMyLinkedList::InsertAtCurrent(cPerson newPerson)
{
	// Edge case: Is this the 1st node?
	if (this->pCurrentNode == nullptr)
	{
		// This ISN'T pointing to anything
		// so make one
		this->pCurrentNode = new cNode();
		// cNode* pCurrentNode = new cNode();

		// Add the data to it
		this->pCurrentNode->thePerson = newPerson;
		return;	
	}
		
	// If we are here, there is a valid current node
	// i.e. this is the 2nd (or later) insert

	cNode* pTempNode = new cNode();
	pTempNode->thePerson = newPerson;

	// Point this NEW node back to the prior node
	pTempNode->pPriorNode = this->pCurrentNode;

	// Connect this node to the current node
	this->pCurrentNode->pNextNode = pTempNode;
	


	// Move the current node to this new node
	this->pCurrentNode = pTempNode;

	return;
}

cPerson cMyLinkedList::GetAtCurrent(void)
{
	// TODO: Is list empty??
	return this->pCurrentNode->thePerson;
}

bool cMyLinkedList::MoveNext(void)
{
	// We are at the "current" node
	// Does the "next" node exist (doesn't point to null)?
	if (this->pCurrentNode->pNextNode == nullptr)
	{
		// No, there ISN'T another node
		// So, we are at the "last" node (the "tail")
		return false;
	}
	// At this point, there IS a next node
	// ...so move to it
	this->pCurrentNode = this->pCurrentNode->pNextNode;
	return true;
}

bool cMyLinkedList::MovePrevious(void)
{
	if (this->pCurrentNode->pPriorNode == nullptr)
	{
		// No, this is the 1st node
		return false;
	}

	this->pCurrentNode = this->pCurrentNode->pPriorNode;
	return true;
}
