#pragma once
#include "cPerson.h"

class cMyStack
{
public:
	// Adds to "top" of stack
	void Push(cPerson newPerson);
	// Removes (and returns) 
	//  item at top of stack
	cPerson Pop(void);


	// Some other helpful methods:
	cPerson Peek(void);	// Look at top, but not remove
	bool IsEmpty(void);
	unsigned int GetCapacity(void);


private:
	int m_topOfStackIndex = 0;
	// TODO: Make this dynamic so can resize
	static const int ARRAYSIZE = 1000;
	cPerson m_myData[cMyStack::ARRAYSIZE];
};