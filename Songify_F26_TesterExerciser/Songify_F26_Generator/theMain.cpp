#include <string>
#include <iostream>
#include "cMusicGenerator.h"
#include "cPersonGenerator.h"
#include <chrono>
#include <sstream>

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// Assumes you're passing 6 characters
std::string printablePostalCode(char postalCode[/*6*/]);
// Assumes you're passing 11 characters
// NOTE: Stored as NUMBERS *NOT* ASCII values (like the postal code)
std::string printablePhoneNumber(char phoneNumber[/*11*/]);
// Social Insurance Numbers
// https://en.wikipedia.org/wiki/Social_insurance_number or https://www.canada.ca/en/employment-social-development/services/sin.html
std::string printableSIN(unsigned int numSIN);

int main(int argc, char* argv[])
{

//	char phone[11] = { 1, 5, 1, 9, 4, 3, 0, 1, 2, 3, 4 };
//	std::cout << printablePhoneNumber(phone);

	cMusicGenerator* pMG = new cMusicGenerator();

	std::chrono::high_resolution_clock timer;
	std::chrono::steady_clock::time_point t1 = timer.now();
	
#ifdef USE_CALLBACK_PROCESS_OBJECT	
	// This is optional, if you want to see how the loading is going... 
	cCallbackProgress progressObject;
	pMG->setProgressCallbackObjectInstance(&progressObject);
#endif


	std::string errorString;
	if ( ! pMG->LoadMusicInformation("Data/billboard/kcmillersean-billboard-hot-100-1958-2017/data/audiio.csv", errorString) )
	{ 
		std::cout << "Didn't load file because: " << errorString << std::endl;
		return -1;
	}
	std::cout << "Billboard music file loaded OK" << std::endl;

	std::cout << "Loaded " << pMG->getNumberOfSongsLoaded() << " songs";
	
	std::chrono::steady_clock::time_point t2 = timer.now();
	std::cout << " in " << std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << " seconds" << std::endl;

	std::cout << std::endl;

	// Ten randomly chosen songs:
	std::cout << "10 randomly picked songs:" << std::endl;
	for (unsigned int count = 0; count != 10; count++ )
	{ 
		cSong aSong = pMG->getRandomSong();
		std::cout
			<< "ID: [" << aSong.getUniqueID() << "] "
			<< aSong.name << " "
			<< "by: " << aSong.artist << std::endl;
	}

	std::cout << std::endl;
	std::cout << "10 randomly made up songs:" << std::endl;
	for (unsigned int count = 0; count != 10; count++)
	{
		cSong aSong = pMG->getRandomMadeUpSong();
		std::cout
			<< "ID: [" << aSong.getUniqueID() << "] "
			<< aSong.name << " "
			<< "by: " << aSong.artist << std::endl;
	}

	// Find song
	std::cout << std::endl;

	std::vector<cSong> vecFoundSongs;

	// Return all the songs currently present (including made up ones)
	pMG->findSong("", "", vecFoundSongs, false);

	std::cout << "Currently " << vecFoundSongs.size() << " songs in generator" << std::endl;


	vecFoundSongs.clear();
	pMG->findSong("", "Taylor Swift", vecFoundSongs, false);
	std::cout << "Currently " << vecFoundSongs.size() << " songs by \'Taylor Swift\'" << std::endl;

	vecFoundSongs.clear();
	pMG->findSong("", "Taylor Swift", vecFoundSongs, true);
	std::cout << "Currently " << vecFoundSongs.size() << " songs by \'Taylor Swift\' (search is NOT case sensitive)" << std::endl;


//	if (pMG->findSong("Shake it off", "Taylor Swift", vecFoundSongs))
	if (pMG->findSong("Afterglow", "Taylor Swift", vecFoundSongs))
	{
		std::cout << "Found " << vecFoundSongs.size() << " match(es)!" << std::endl;

		for ( cSong &songMatch : vecFoundSongs)
		std::cout
			<< "[" << songMatch.name << "]"
			<< " by "
			<< "[" << songMatch.artist << "]"
			<< " uniqueID: " << songMatch.getUniqueID()
			<< std::endl;
	}
	else
	{
		std::cout << "Sorry, didn't find a match for that song." << std::endl;
	}

	std::cout << std::endl;

	cPersonGenerator* pPG = new cPersonGenerator();

	// babyNameFile is one of the files in "Popular Baby Names/names"
	//	yob2019.txt is "year of birth 2019" for example
	// 
	// surnameFile is "Surnames/names/Names_2010Census.csv"
	// 
	// streetNameFile is "Street Names (San Francisco)/Street_Names.csv"
	//
	std::string babyNameFile = "Data/Popular Baby Names/names/yob2019.txt";
	std::string surnameFile = "Data/Surnames/names/Names_2010Census.csv";
	std::string streetNameFile = "Data/Street Names (San Francisco)/Street_Names.csv";

	t1 = timer.now();

	std::cout << "Loading the census data into person generator...";
	if (!pPG->LoadCensusFiles(babyNameFile, surnameFile, streetNameFile, errorString))
	{
		std::cout 
			<< "There was a problem loading one or more files because: "
			<< errorString << std::endl;
		return -1;
	}

	t2 = timer.now();
	std::cout 
		<< "done in "
		<< std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << " seconds" << std::endl;

	std::cout << std::endl;

	std::cout << "Generating some random people:" << std::endl;
	std::cout << std::endl;

	for (unsigned int count = 0; count != 10; count++)
	{
		cPerson aPerson = pPG->generateRandomPerson(true);

		std::cout
			<< aPerson.first << " "
			<< aPerson.last
			<< ", age: " << aPerson.age
			<< ", identifies as: " << aPerson.getGenderAsString() << std::endl
			<< "primary phone# : " << printablePhoneNumber(aPerson.phoneNumbers[0]) << std::endl
			<< "Address:" << std::endl
			<< aPerson.streetNumber << " "
			<< aPerson.streetName << " "
			<< aPerson.streetType << " "
			<< aPerson.streetDirection << std::endl
			<< aPerson.city << ", " << aPerson.province << std::endl
			<< printablePostalCode(aPerson.postalCode) << std::endl
			<< std::endl;
	}


	// Stress test: generate a lot of songs and people
	const unsigned int NUM_PEOPLE_STRESS = 1'000'000;
	// number of dots for progress
	const unsigned int NUM_PEOPLE_STRESS_DOTS = NUM_PEOPLE_STRESS / 100;

	std::stringstream ssProgress;
	ssProgress << "Generating " << NUM_PEOPLE_STRESS << " people ";
	std::string baseProgressString = ssProgress.str();
	for (unsigned int count = 0; count != NUM_PEOPLE_STRESS; count++)
	{
		cPerson aPerson = pPG->generateRandomPerson(true);
		if (count % NUM_PEOPLE_STRESS_DOTS == 0)
		{
			// Clear text
			for (const char c : ssProgress.str())
			{
				std::cout << '\b';
			}
			unsigned int percentDone = count / NUM_PEOPLE_STRESS_DOTS;
			ssProgress.str("");
			ssProgress << baseProgressString << "(" << percentDone << "%)";
			std::cout << ssProgress.str();
		}
	}
	std::cout << ". Done." << std::endl;

	const unsigned int NUM_SONGS_STRESS = 1'000'000'000;
	// number of dots for progress
	const unsigned int NUM_SONGS_STRESS_DOTS = NUM_SONGS_STRESS / 100;

	ssProgress.str("");
	ssProgress << "Generating " << NUM_SONGS_STRESS << " songs";
	baseProgressString = ssProgress.str();
	std::cout << ssProgress.str();
	for (unsigned int count = 0; count != NUM_SONGS_STRESS; count++)
	{
		cSong aSong = pMG->getRandomSong();
		if (count % NUM_SONGS_STRESS_DOTS == 0)
		{
			// Clear text
			for (const char c : ssProgress.str())
			{
				std::cout << '\b';
			}
			unsigned int percentDone = count / NUM_SONGS_STRESS_DOTS;
			ssProgress.str("");
			ssProgress << baseProgressString << "(" << percentDone << "%)";
			std::cout << ssProgress.str();
		}
	}
	std::cout << ". Done." << std::endl;


	delete pMG;
	delete pPG;


	return 0;
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