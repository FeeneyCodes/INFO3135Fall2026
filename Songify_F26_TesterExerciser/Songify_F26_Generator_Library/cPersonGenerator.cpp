#include "cPersonGenerator.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// Warning C26812 : Prefer 'enum class' over 'enum' (Enum.3)
#pragma warning( disable : 26812 )

#include <iostream>
#include <fstream>
#include <sstream>		// String Stream
#include <string>

#include "cPersonGenerator_Imp.h"

cPersonGenerator::cPersonGenerator()
{
	this->p_Imp = new cPersonGenerator_Imp();
}

cPersonGenerator::~cPersonGenerator()
{
	delete this->p_Imp;
}

// This one assumes that you want to load
// - Data/Popular Baby Names/names/yob2019.txt
// - Data/Street Names (San Francisco)/Street_Names.csv
// - Data/Surnames/names/Names_2010Census.csv
bool cPersonGenerator::LoadCensusFiles(std::string& errorString)
{
	return this->p_Imp->LoadCensusFiles("", "", "", errorString);
}

// Randomly generates a person from the data
cPerson cPersonGenerator::generateRandomPerson(bool bBasedOnPopulationDistribution /*=false*/)
{
	return this->p_Imp->generateRandomPerson(bBasedOnPopulationDistribution);
}


unsigned int cPersonGenerator::getNumberOfNamesLoaded(void)
{
	return this->p_Imp->getNumberOfNamesLoaded();
}

unsigned int cPersonGenerator::getNumberOfSurnamesLoaded(void)
{
	return this->p_Imp->getNumberOfStreetsLoaded();
}

unsigned int cPersonGenerator::getNumberOfStreetsLoaded(void)
{
	return this->p_Imp->getNumberOfStreetsLoaded();
}



bool cPersonGenerator::LoadCensusFiles(
	std::string babyNameFile,
	std::string surnameFile,
	std::string streetNameFile,
	std::string& errorString)
{
	return this->p_Imp->LoadCensusFiles(babyNameFile, surnameFile, streetNameFile, errorString);
}
































// Here's a simple way to load the comma delimited files:
bool readCSVFile(void)
{
	// Open the file
	std::ifstream namesFile("Names_2010Census.csv");
	if (!namesFile.is_open())
	{
		std::cout << "Didn't open file" << std::endl;
		return false;
	}

	// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
	// SMITH,1,2442977,828.19,828.19,70.9,23.11,0.5,0.89,2.19,2.4
	//
	// - rank is how popular the last name is, like 1st, 2nd, etc.
	// - count is how many people have that last name, so 2,442,977 people have last name "Smith"
	// - prop100k is the ratio per 100,000 people. Smith is 828.19, 
	//            meaning that there's a 828.19 out of 100,000 chance (0.82819% chance)
	//            that someone is named "Smith"

	std::string theLine;

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if (tokenCount == 0)
			{
				std::cout << token << std::endl;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;

	return true;
}