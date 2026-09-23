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
private:
	static const int ARRAYSIZE = 1000;
	cPerson m_myData[cMyStack::ARRAYSIZE];
};