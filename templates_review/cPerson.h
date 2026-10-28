// A classic type of header guard
#ifndef _cPerson_HG_2020_09_26_
#define _cPerson_HG_2020_09_26_

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

#include <string>

class cPerson
{
public:
	cPerson();		// constructor (c'tor)
	// Note: Think if you want this virtual or not...?
	// if polymorphic (i.e. will inherit), the 
	//	MAKE it a virtual destructor or you'll get a memory leak
	// Right now, it is NOT polymorphic so the destructor ISN'T virtual.
	~cPerson();		// destructor (d'tor)

	std::string first;
	std::string middle;
	std::string last;


	// enum inside the class "tightly coupled"
	enum eGenderType
	{
		MALE = 0,
		FEMALE,
		NON_BINARY,
		RATHER_NOT_SAY_UNKNOWN
	};

	eGenderType gender;
	std::string getGenderAsString(void);

	int age;		// default = -1;

	// These are in the format from the US census data: https://catalog.data.gov/dataset/street-names-37fec/resource/d655cc82-c98f-450a-b9bb-63521c870503
	//
	// Something like this: "MISSION BAY BLVD NORTH, MISSION BAY, BLVD, NORTH"
	// would be:
	//	- streetName = "MISSION BAY"
	//  - streetType = "BLVD"
	//  - streetDirection = "NORTH"	    Note: many streets don't have a streetDirection
	//                                        and "street direction" can be things like "RAMP", etc. 
	// 
	int streetNumber;		// default = 0 
	std::string streetName;
	std::string streetType;
	std::string streetDirection;

	std::string city;
	std::string province;
	
	// All zeros if not known or invalid
	// Stored as ASCII values.
	char postalCode[6];		// Canadian postal codes are 6 characters

	// This would be:
	//     1 digit for country code
	//     3 digits for area code
	//     7 gigits for the actual number
	// eg: 1-519-452-4277  (which is Fanshawe's main number)
	// 
	// There are up to four numbers (home, mobile, etc.)
	// If they only have one number, place it in phoneNumbers[0].
	// If they don't have a number, it's all zeros (0)
	//
	// NOTE: Stored as NUMBERS *NOT* ASCII values (like the postal code)
	char phoneNumbers[4][11];
	unsigned int SIN;	// = 0
	//unsigned int SIN = 0;		// C++ 11

	unsigned int getSongifyUniqueUserID(void);

private:
	unsigned int m_Songify_UniqueUserID;
	// 
	static unsigned int m_NEXT_Songify_UniqueUSerID;
};

#endif
