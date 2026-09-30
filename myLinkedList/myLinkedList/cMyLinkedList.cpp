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
	// TODO: Check if the list is empty

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
	// TODO: Check if list is empty

	if (this->pCurrentNode->pPriorNode == nullptr)
	{
		// No, this is the 1st node
		return false;
	}

	this->pCurrentNode = this->pCurrentNode->pPriorNode;
	return true;
}


bool cMyLinkedList::DeleteAtCurrent(void)
{
	// TODO: Check if list is empty

	// Prior Node
	//   |    ^       |  ^
	//   V    |       |  |
	// current Node   |  |
	//   |    ^       |  |
	//   V    |       V  |
	// Next Node

	cNode* pNodeToDelete = this->pCurrentNode;

	// Change the "current node's PRIOR next node"
	//	to point to the "current node's next node"
	if (pNodeToDelete->pPriorNode != nullptr)
	{
		// It's NOT the head (1st node)
		// i.e. there IS a node prior
		pNodeToDelete->pPriorNode->pNextNode = pNodeToDelete->pNextNode;
	}

	// The next node's prior node is now pointing
	//	to the current node's prior node
	if (pNodeToDelete->pNextNode != nullptr)
	{
		// It's NOT the tail (last node)
		// i.e. there IS a node after this one
		pNodeToDelete->pNextNode->pPriorNode = pNodeToDelete->pPriorNode;
	}

	// Update the 'current' node:
	// TODO: WAS this the 1st node?
	// TODO: WAS this the last node?

	// We'll make the current node the deleted node's next node
	this->pCurrentNode = pNodeToDelete->pNextNode;

	// Now we can delete the node
	delete pNodeToDelete;

	return true;
}

