#include "formattingHelperFunctions.h"
#include <sstream>

// These are just some helper functions that (hopefully)
//	help show how the data is being stored in the cPerson
//	class and how it can be printed out nicely.

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//


std::string printableSong(cSong& theSong)
{
	std::stringstream ssSong;
	ssSong
		<< theSong.name << " by " << theSong.artist
		<< " (" << theSong.releaseDateYear << "-" << theSong.releaseDateMonth << "-" << theSong.releaseDateDay << ")"
		<< " ID:" << theSong.getUniqueID()
		<< " rating:" << theSong.rating
		<< " played:" << theSong.numberOfTimesPlayed << " times";
	return ssSong.str();
}

std::string printablePerson(cPerson& thePerson, bool bOneLine /*=false*/)
{
	std::stringstream ssPerson;
	ssPerson << thePerson.first << " " << thePerson.middle << " " << thePerson.last;

	if (bOneLine)
	{
		// Only prints 1st phone number
		ssPerson
			<< " SIN:" << printableSIN(thePerson.SIN)
			<< ", aged: " << thePerson.age
			<< " identifies as: " << thePerson.getGenderAsString()
			<< " @ "
			<< thePerson.streetName << " " << thePerson.streetName << " "
			<< thePerson.streetType << " " << thePerson.streetDirection << ", "
			<< thePerson.city << ", " << thePerson.province << " "
			<< printablePostalCode(thePerson.postalCode) << " "
			<< printablePhoneNumber(thePerson.phoneNumbers[0]);// << '\n';
	}
	else
	{	// Multi-line
		ssPerson << '\n';
		ssPerson
			<< " SIN:" << printableSIN(thePerson.SIN)
			<< ", aged: " << thePerson.age
			<< " identifies as: " << thePerson.getGenderAsString() << '\n';
		ssPerson
			<< thePerson.streetName << " " << thePerson.streetName << " "
			<< thePerson.streetType << " " << thePerson.streetDirection << '\n'
			<< thePerson.city << ", " << thePerson.province << '\n'
			<< printablePostalCode(thePerson.postalCode) << '\n';
		for (unsigned int index = 0; index != 4; index++)
		{
			ssPerson
				<< "phone[" << index << "]: " << printablePhoneNumber(thePerson.phoneNumbers[index]) << '\n';
		}
	}


	return ssPerson.str();
}

// Assumes you're passing 6 characters
std::string printablePostalCode(char postalCode[/*6*/])
{
	std::stringstream ssPostalCode;

	ssPostalCode
		<< postalCode[0]
		<< postalCode[1]
		<< postalCode[2]
		<< ' '
		<< postalCode[3]
		<< postalCode[4]
		<< postalCode[5];

	return ssPostalCode.str();
}

// Assumes you're passing 11 characters
// NOTE: Stored as NUMBERS *NOT* ASCII values (like the postal code)
std::string printablePhoneNumber(char phoneNumber[/*11*/])
{
	std::stringstream ssPhoneNum;

	ssPhoneNum
		<< '+'
		<< (char)(phoneNumber[0] + '0')
		<< " ("
		<< (char)(phoneNumber[1] + '0')
		<< (char)(phoneNumber[2] + '0')
		<< (char)(phoneNumber[3] + '0')
		<< ") "
		<< (char)(phoneNumber[4] + '0')
		<< (char)(phoneNumber[5] + '0')
		<< (char)(phoneNumber[6] + '0')
		<< '-'
		<< (char)(phoneNumber[7] + '0')
		<< (char)(phoneNumber[8] + '0')
		<< (char)(phoneNumber[9] + '0')
		<< (char)(phoneNumber[10] + '0');

	return ssPhoneNum.str();
}


// Social Insurance Numbers
// https://en.wikipedia.org/wiki/Social_insurance_number or https://www.canada.ca/en/employment-social-development/services/sin.html
std::string printableSIN(unsigned int numSIN)
{
	std::stringstream ssSIN;
	ssSIN << numSIN;
	std::string sSIN = ssSIN.str();

	// pad left with '0's
	while (sSIN.length() < 9)
	{
		sSIN.insert(0, "0");
	}
	// SIN is now 9 digits with leading zeros
	sSIN.insert(3, " ");
	sSIN.insert(7, " ");

	return sSIN;
}