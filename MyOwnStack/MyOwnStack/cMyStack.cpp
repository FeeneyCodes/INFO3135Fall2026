#include "cMyStack.h"

// Adds to "top" of stack
void cMyStack::Push(cPerson newPerson)
{
	// TODO: Check to see if stack is full?

	// 1. Add item at "top of stack index"
	this->m_myData[ this->m_topOfStackIndex ] = newPerson;

	// 2. Increment "top of stack index"
	this->m_topOfStackIndex++;

	return;
}

// Removes (and returns) 
//  item at top of stack
cPerson cMyStack::Pop(void)
{
	// 1. Decrement the "top of stack index"
	this->m_topOfStackIndex--;

	// 2. Return that value
	return this->m_myData[ this->m_topOfStackIndex ];
}