#ifndef _cPersonGenerator_HG_
#define _cPersonGenerator_HG_

#include <string>
#include "cPerson.h"


// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// Forward declare for the implementation
class cPersonGenerator_Imp;

class cPersonGenerator
{
public:
	cPersonGenerator();
	~cPersonGenerator();

	// This takes three files:
	// - One of the "Popular Baby Names" found here: https://www.ssa.gov/oact/babynames/limits.html
	//   (example: "yob2001.txt" which are babies born in the year 2001
	// - The "Names_2010Census.csv" file, from:
	//   "Frequently Occurring Surnames from the 2010 Census" from here:
	//   https://www.census.gov/topics/population/genealogy/data/2010_surnames.html
	// - The "Street_Names.csv" file, from:
	//   "Street Names (San Francisco)" from here: 
	//   https://catalog.data.gov/dataset/street-names-37fec/resource/d655cc82-c98f-450a-b9bb-63521c870503
	// Returns:
	// - true of successful
	// - false if there's any issue (can't find file, or whatever)
	//   If false, then the "errorString" (passed by reference) may give more information
	//
	// "babyNameFile" is one of the files in "Popular Baby Names/names"
	//           example: "yob2019.txt" is "year of birth 2019" for example
	// 
	// "surnameFile" is "Surnames/names/Names_2010Census.csv"
	// 
	// "streetNameFile" is "Street Names (San Francisco)/Street_Names.csv"
	//

	bool LoadCensusFiles(
		std::string babyNameFile,
		std::string surnameFile,
		std::string streetNameFile,
		std::string& errorString);

	// This one assumes that you want to load
	// - Data/Popular Baby Names/names/yob2019.txt
	// - Data/Street Names (San Francisco)/Street_Names.csv
	// - Data/Surnames/names/Names_2010Census.csv
	bool LoadCensusFiles(std::string& errorString);

	// Randomly generates a person from the data
	// Basing it on the population distribution is trivially slower,
	//	but gives more accurate sets of names.
	// Making it false treats every name as equaliy likely 
	// (so you'll get more rare names)
	cPerson generateRandomPerson(bool bBasedOnPopulationDistribution = false);


	unsigned int getNumberOfNamesLoaded(void);
	unsigned int getNumberOfSurnamesLoaded(void);
	unsigned int getNumberOfStreetsLoaded(void);


private:
	cPersonGenerator_Imp* p_Imp = NULL;

};


#endif 
