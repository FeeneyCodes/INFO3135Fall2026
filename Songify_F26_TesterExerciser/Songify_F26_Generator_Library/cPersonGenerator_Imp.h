#ifndef _cPersonGenerator_Imp_HG_
#define _cPersonGenerator_Imp_HG_

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

#include <string>
#include "cPerson.h"
#include <vector>
#include "cMersenneTwister.h"

#include "sCanadianCityInfo.h"

class cPersonGenerator_Imp
{
public:
	cPersonGenerator_Imp();
	~cPersonGenerator_Imp();


	// If any entries are blank, it assumes:
	// - Data/Popular Baby Names/names/yob2019.txt
	// - Data/Street Names (San Francisco)/Street_Names.csv
	// - Data/Surnames/names/Names_2010Census.csv
	bool LoadCensusFiles(
		std::string babyNameFile,
		std::string surnameFile,
		std::string streetNameFile,
		std::string& errorString);

	// Randomly generates a person from the data
	cPerson generateRandomPerson(bool bBasedOnPopulationDistribution);


	unsigned int getNumberOfNamesLoaded(void);
	unsigned int getNumberOfSurnamesLoaded(void);
	unsigned int getNumberOfStreetsLoaded(void);





	bool LoadBabyNameFile(std::string babyNameFile, std::string& errorString);
	bool LoadSurnameFile(std::string surnameFile, std::string& errorString);
	bool LoadStreetNameFile(std::string streetNameFile, std::string& errorString);

	// Used for parsing the CSV files
	unsigned int tokenizeLineWithQuotes(const std::string& theString, std::vector<std::string>& vecTokens);

	// Olivia,F,18451
	struct sBabyNameInfo
	{
		std::string rawLineFromFile;	// as listed in the file
		//
		std::string name;
		char gender = '?';
		unsigned int ranking = 0;
		double ranking_as_percentage = 0.0;		// Calculated
		// Sum of this and all prior rankings
		unsigned int cumulative_ranking = 0;
	};

	// name, rank,count,  prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
	// SMITH,1,   2442977,828.19,  828.19,      70.9,    23.11,   0.5,   0.89,   2.19,     2.4
	struct sSurnameInfo
	{
		std::string rawLineFromFile;	// as listed in the file
		//
		std::string name;
		std::string name_CaseCorrected;	// The "name" but with 1st character upper case
		unsigned int rank = 0;
		unsigned int count = 0;
		
		// These are used for the distributive name selection
		unsigned int cumulative_count = 0;					// Calculated
		double cumulative_proportion_as_percentage = 0.0;	// Calculated

		// Proportion per 100K people? 
		// "SMITH" has a value of 828.19 meaning 828.19 of 100,000 people are named SMITH
		float prop100k = 0.0f;		
		double prop100k_as_percentage = 0.0;	// Calculated
		// Cumulative proportion per 100K people?
		// This is the running total of the population so far in the file.
		// For example: "MARTINEZ" is the 10th most common name with 
		//   359.4 per 100,000 people or 0.003594 or 0.3594% of the population
		//   the cum_prop100k = 4902.9
		// meaning that these "top 10 most common lastnames" make up 
		//	4902.9 per 100,000 people or 0.049029 or 4.9029% of the population
		float cum_prop100k = 0.0f;	
		double cum_prop100k_as_percentage = 0.0;	// Calculated

		//
		// OK, these are so strange, but it's what the US census takes, I guess??
		// These are mostly guesses based on what I could google and what's on this site:
		//	https://www.census.gov/library/visualizations/interactive/race-and-ethnicity-in-the-united-state-2010-and-2020-census.html
		// 
		// Not that it matters, since we aren't using any of these numbers, anyway.
		//
		float pctwhite = 0.0f;		// Percentage white?  
		float pctblack = 0.0f;		// Percentage black?
		float pctapi = 0.0f;		// No idea what this is
		float pctaian = 0.0f;		// Percentage asian?
		float pct2prace = 0.0f;		// No idea what this is
		float pcthispanic = 0.0f;	// Percentage hispanic?
	};

	// FullStreetName,StreetName,StreetType,PostDirection
	// BATTERY WAGNER RD,BATTERY WAGNER,RD,
	// MARK TWAIN PL,MARK TWAIN,PL,
	// MARKET ST, MARKET, ST,		<-- the 'main' street in San Francisco
	// MONETA WAY,MONETA,WAY,
	// RACINE LN,RACINE,LN,
	struct sStreetNameInfo
	{
		std::string rawLineFromFile;	// as listed in the file
		//
		std::string FullStreetName;
		std::string StreetName;
		// std::string StreetName_CaseCorrected;		// 1st letter Upper, rest lower case
		std::string StreetType;
		// std::string StreetType_CaseCorrected;		// 1st letter Upper, rest lower case
		std::string PostDirection;
		// std::string PostDirection_CaseCorrected;	// 1st letter Upper, rest lower case
	};

	std::vector<sBabyNameInfo> vecBabyNames;
	// This is the total number of babies in the census.
	// For example: "Olivia,F,18451" means that there were 
	//  18,451 babies named "Olivia" that year.
	// This is the sum of all of those numbers.
	// This is used when picking names so that the distribution of names is more accurate
	unsigned int totalBabyNameCount = 0;
	std::string getRandomFirstName(bool bUseDistribution = true);

	std::vector<sSurnameInfo> vecSurnames;
	// Same idea as the baby names, above ^^
	unsigned int totalSurnameCount = 0;
	std::string getRandomLastName(bool bUseDistribution = true);

	std::vector<sStreetNameInfo> vecStreetNames;

	enum eProvince
	{
		MANITOBA = 0,					// "Manitoba"
		NEW_BRUNSWICK,					// "New Brunswick"
		NEWFOUNDLAND_AND_LABRADOR,		// "Newfoundland and Labrador"
		ONTARIO,						// "Ontario"
		QUEBEC,							// "Québec"
		SASKATCHEWAN,					// "Saskatchewan"
		SANTAS_VILLAGE,					// "North Pole"
		UNKNOWN_OR_UNDEFINED
	};

	std::string eProvinceAsString(eProvince province);

	std::vector<sCanadianCityInfo> vecCanadianCities;
	// Used for selecting population based on distribution
	unsigned int totalCanadianCityPopulation = 0;
	sCanadianCityInfo getRandomCity(bool bUseDistribution = true);

	cMersenneTwister* p_theRandomizer;	// = NULL;
};


#endif
