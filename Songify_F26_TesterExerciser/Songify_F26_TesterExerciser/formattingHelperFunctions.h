#pragma once

#include <string>
#include "StudentSolutionFiles/cPerson.h"
#include "StudentSolutionFiles/cSong.h"

// Assumes you're passing 6 characters
std::string printablePostalCode(char postalCode[/*6*/]);
// Assumes you're passing 11 characters
// NOTE: Stored as NUMBERS *NOT* ASCII values (like the postal code)
std::string printablePhoneNumber(char phoneNumber[/*11*/]);
// Social Insurance Numbers
// https://en.wikipedia.org/wiki/Social_insurance_number or https://www.canada.ca/en/employment-social-development/services/sin.html
std::string printableSIN(unsigned int numSIN);
// If only one line, then only prints 1st phone number
std::string printablePerson(cPerson& thePerson, bool bOneLine = false);

std::string printableSong(cSong& theSong);
