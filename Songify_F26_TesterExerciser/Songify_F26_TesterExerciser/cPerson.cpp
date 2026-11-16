#include "cPerson.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

cPerson::cPerson()
{
	// In here, set the default information
	this->gender = cPerson::RATHER_NOT_SAY_UNKNOWN;
	this->streetNumber = 0;
	this->age = -1;
	this->SIN = 0;

	memset(this->postalCode, 0, 6);
	memset(this->phoneNumbers[0], 0, 11);
	memset(this->phoneNumbers[1], 0, 11);
	memset(this->phoneNumbers[2], 0, 11);
	memset(this->phoneNumbers[3], 0, 11);

	// NOTE: This is just some filler code that you can use so you
	//       get unique values. You should use the value that's
	//       given to you by the person generator code. 
	//
	// Also be careful if you decide to override the copy or assignment
	//	operators because this "uniqueID" is what determines exactly
	//	who this person is. Like your student or customer number.


	// Generate unique Songify ID
	this->m_Songify_UniqueUserID = cPerson::m_NEXT_Songify_UniqueUSerID;
	// Increment for next created user by a small random amount 
	const unsigned int MAX_ID_INCREMENT = 11;
	cPerson::m_NEXT_Songify_UniqueUSerID += (rand() % MAX_ID_INCREMENT) + 1;
}

// The 1st Sonify user will have ID: 10,000,000
// static 
unsigned int cPerson::m_NEXT_Songify_UniqueUSerID = 10000000;


cPerson::~cPerson()
{
	
}

std::string cPerson::getGenderAsString(void)
{
	switch (this->gender)
	{
	case cPerson::MALE:
		return "MALE";
		break;
	case cPerson::FEMALE:
		return "FEMALE";
		break;
	case cPerson::NON_BINARY:
		return "NON_BINARY";
		break;
	case cPerson::RATHER_NOT_SAY_UNKNOWN:
		return "RATHER_NOT_SAY_UNKNOWN";
		break;
//	default:
//		break;
	}

	// This should never happen
	return "OMG! CALL THE DEV TEAM!! SOMEONE SCREWED UP!!!";
}

unsigned int cPerson::getSongifyUniqueUserID(void)
{
	return this->m_Songify_UniqueUserID;
}
