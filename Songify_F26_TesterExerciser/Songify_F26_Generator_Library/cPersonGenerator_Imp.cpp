#include "cPersonGenerator_Imp.h"
#include <fstream>
#include <sstream>

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// For sanity (i.e. less typing)
typedef  cPersonGenerator_Imp cPG;

#include "sCanadianCityInfo_Data.h"

cPersonGenerator_Imp::cPersonGenerator_Imp()
{
	// Load the Canadian cities data...
	this->totalCanadianCityPopulation = 0;
	this->vecCanadianCities.reserve(::g_NumberOfCitiesInCanadianCitiesArray);
	for (unsigned int index = 0; index != ::g_NumberOfCitiesInCanadianCitiesArray; index++)
	{
		this->totalCanadianCityPopulation += ::g_CitiesInCanadaArray[index].population;
		::g_CitiesInCanadaArray[index].cumulative_population = this->totalCanadianCityPopulation;
		this->vecCanadianCities.push_back(::g_CitiesInCanadaArray[index]);
	}

	this->p_theRandomizer = new cMersenneTwister((unsigned long)time(NULL));
}


cPersonGenerator_Imp::~cPersonGenerator_Imp()
{

}

// If any entries are blank, it assumes:
// - Data/Popular Baby Names/names/yob2019.txt
// - Data/Street Names (San Francisco)/Street_Names.csv
// - Data/Surnames/names/Names_2010Census.csv
bool cPersonGenerator_Imp::LoadCensusFiles(
	std::string babyNameFile,
	std::string surnameFile,
	std::string streetNameFile,
	std::string& errorString)
{
	// Load defaults if any of these are blank
	if (babyNameFile == "")
	{
		babyNameFile = "Data/Popular Baby Names/names/yob2019.txt";
	}
	if (surnameFile == "")
	{
		surnameFile = "Data/Surnames/names/Names_2010Census.csv";
	}
	if (streetNameFile == "")
	{
		streetNameFile = "Data/Street Names (San Francisco)/Street_Names.csv";
	}

	bool bAnyLoadErrors = false;

	std::string babyNameLoadErrors;
	if ( ! this->LoadBabyNameFile(babyNameFile, babyNameLoadErrors) )
	{
		bAnyLoadErrors = true;
		errorString += "Error loading baby name file: ";
		errorString += babyNameLoadErrors;
		errorString += '\n';
	}

	std::string surnameLoadErrors;
	if ( ! this->LoadSurnameFile(surnameFile, surnameLoadErrors) )
	{
		bAnyLoadErrors = true;
		errorString += "Error loading surnames file: ";
		errorString += surnameLoadErrors;
		errorString += '\n';
	}

	std::string streetNamesLoadErrors;
	if ( ! this->LoadStreetNameFile(streetNameFile, streetNamesLoadErrors) )
	{
		bAnyLoadErrors = true;
		errorString += "Error loading street names file: ";
		errorString += streetNamesLoadErrors;
		errorString += '\n';
	}

	return ! bAnyLoadErrors;
}

unsigned int cPersonGenerator_Imp::tokenizeLineWithQuotes(const std::string& theString, std::vector<std::string>& vecTokens)
{
	vecTokens.clear();

	bool bInsideAQuote = false;
	std::string curToken;
	for (unsigned int index = 0; index != theString.size(); index++)
	{
		char curChar = theString[index];

		if (curChar == '\"')
		{
			// Flip flag
			bInsideAQuote = !bInsideAQuote;
		}
		else if (curChar == ',')
		{
			// inside a quote
			if (bInsideAQuote)
			{
				// Yes, so ignore it
				curToken.push_back(curChar);
			}
			else
			{
				// Token done
				vecTokens.push_back(curToken);
				curToken = "";
			}
		}
		else
		{
			curToken.push_back(curChar);
		}

	}//for (unsigned int index...

	// Add the very last one in the line (there is no end "," character)
	vecTokens.push_back(curToken);

	return (unsigned int)vecTokens.size();
}

bool cPersonGenerator_Imp::LoadBabyNameFile(std::string babyNameFileName, std::string& errorString)
{
	std::ifstream babyNameFile(babyNameFileName.c_str());
	if (!babyNameFile.is_open())
	{
		errorString = "Can't open file: ";
		errorString += babyNameFileName;
		return false;
	}

	babyNameFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = babyNameFile.tellg();
	// Return to start
	babyNameFile.seekg(0, std::ios::beg);

	char* pRawBabyNameFileData = new char[static_cast<unsigned int>(fileSize)];

	babyNameFile.read(pRawBabyNameFileData, fileSize);
	babyNameFile.close();

	// Create stream for getline()
	std::stringstream ssFileData(std::string(pRawBabyNameFileData, (std::size_t)fileSize));

	// *********************

	unsigned int lineCount = 0;
	sBabyNameInfo curBabyNameLine;
	while (std::getline(ssFileData, curBabyNameLine.rawLineFromFile))
	{
		lineCount++;

//		// 1st line has header info, so skip
//		if (lineCount == 1)
//		{
//			continue;
//		}
		if (curBabyNameLine.rawLineFromFile.empty())
		{
			continue;
		}
		// HACK:
		// Sometimes the last line is garbage.
		// See if this line is WAY too long
		// I'm assuming this is something to do with the parsting the stringstream object
		if (curBabyNameLine.rawLineFromFile.length() > 1000)
		{
			continue;
		}

		std::vector<std::string> vecTokens;
		if (this->tokenizeLineWithQuotes(curBabyNameLine.rawLineFromFile, vecTokens) > 0)
		{

			curBabyNameLine.name = vecTokens[0];
			// Grab only 1st character
			curBabyNameLine.gender = vecTokens[1][0];	
			curBabyNameLine.ranking = atoi(vecTokens[2].c_str());
			
			this->totalBabyNameCount += curBabyNameLine.ranking;

			this->vecBabyNames.push_back(curBabyNameLine);
		}

	}//while (std::getline(...

	// Update the calculated stats
	unsigned int cumulativeRanking = 0;
	for (sBabyNameInfo& curBabyName : this->vecBabyNames)
	{
		curBabyName.ranking_as_percentage = (double)curBabyName.ranking / (double)this->totalBabyNameCount;

		cumulativeRanking += curBabyName.ranking;
		curBabyName.cumulative_ranking = cumulativeRanking;
	}

	delete[] pRawBabyNameFileData;

	return true;
}

bool cPersonGenerator_Imp::LoadSurnameFile(std::string surnameFileName, std::string& errorString)
{
	std::ifstream surnamesFile(surnameFileName.c_str());
	if (!surnamesFile.is_open())
	{
		errorString = "Can't open file: ";
		errorString += surnameFileName;
		return false;
	}

	surnamesFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = surnamesFile.tellg();
	// Return to start
	surnamesFile.seekg(0, std::ios::beg);

	char* pRawSurnamesFileData = new char[static_cast<unsigned int>(fileSize)];

	surnamesFile.read(pRawSurnamesFileData, fileSize);
	surnamesFile.close();

	// Create stream for getline()
	std::stringstream ssFileData(std::string(pRawSurnamesFileData, (std::size_t)fileSize));

	// *********************

	unsigned int lineCount = 0;
	sSurnameInfo curSurname;
	this->totalSurnameCount = 0;
	while (std::getline(ssFileData, curSurname.rawLineFromFile))
	{
		lineCount++;

		// 1st line has header info, so skip
		if (lineCount == 1)
		{
			continue;
		}

		if (curSurname.rawLineFromFile.empty())
		{
			continue;
		}

		// HACK:
		// Sometimes the last line is garbage.
		// See if this line is WAY too long
		// I'm assuming this is something to do with the parsting the stringstream object
		if (curSurname.rawLineFromFile.length() > 1000)
		{
			continue;
		}

		std::vector<std::string> vecTokens;
		if (this->tokenizeLineWithQuotes(curSurname.rawLineFromFile, vecTokens) > 0)
		{
			if (vecTokens.size() == 11)
			{
				// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
				curSurname.name = vecTokens[0];
				curSurname.rank = atoi(vecTokens[1].c_str());
				curSurname.count = atoi(vecTokens[2].c_str());
				curSurname.prop100k = (float)atof(vecTokens[3].c_str());
				curSurname.cum_prop100k = (float)atof(vecTokens[4].c_str());
				curSurname.pctwhite = (float)atof(vecTokens[5].c_str());
				curSurname.pctblack = (float)atof(vecTokens[6].c_str());
				curSurname.pctapi = (float)atof(vecTokens[7].c_str());
				curSurname.pctaian = (float)atof(vecTokens[8].c_str());
				curSurname.pct2prace = (float)atof(vecTokens[9].c_str());
				curSurname.pcthispanic = (float)atof(vecTokens[10].c_str());

				curSurname.prop100k_as_percentage = (double)curSurname.prop100k / 100'000.0;
				curSurname.cum_prop100k_as_percentage = (double)curSurname.cum_prop100k / 100'000.0;



				curSurname.name_CaseCorrected = curSurname.name;
				for (char& curChar : curSurname.name_CaseCorrected)
				{
					curChar = tolower(curChar);
				}
				// Put the upper case one back
				curSurname.name_CaseCorrected[0] = curSurname.name[0];

				this->totalSurnameCount += curSurname.count;

				curSurname.cumulative_count = this->totalSurnameCount;

				this->vecSurnames.push_back(curSurname);
			}//if (vecTokens.size() == 11)
		}//if (this->tokenizeLineWithQuotes

	}//while (std::getline(...

	// Calculate cumulative distributions (now that we know the total population)
	double dTotalSurnameCount = (double)this->totalSurnameCount;
	for (sSurnameInfo& curSurname : this->vecSurnames)
	{
		curSurname.cumulative_proportion_as_percentage = (double)curSurname.cumulative_count / dTotalSurnameCount;
	}

	delete[] pRawSurnamesFileData;

	return true;
}

bool cPersonGenerator_Imp::LoadStreetNameFile(std::string streetNameFileName, std::string& errorString)
{
	std::ifstream streetNameFile(streetNameFileName.c_str());
	if (!streetNameFile.is_open())
	{
		errorString = "Can't open file: ";
		errorString += streetNameFileName;
		return false;
	}

	streetNameFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = streetNameFile.tellg();
	// Return to start
	streetNameFile.seekg(0, std::ios::beg);

	char* pRawStreetNamesFileData = new char[static_cast<unsigned int>(fileSize)];

	streetNameFile.read(pRawStreetNamesFileData, fileSize);
	streetNameFile.close();

	// Create stream for getline()
	std::stringstream ssFileData(std::string(pRawStreetNamesFileData, (std::size_t)fileSize));

	// *********************

	unsigned int lineCount = 0;
	sStreetNameInfo curStreetName;
	while (std::getline(ssFileData, curStreetName.rawLineFromFile))
	{
		lineCount++;

		// 1st line has header info, so skip
		if (lineCount == 1)
		{
			continue;
		}

		if (curStreetName.rawLineFromFile.empty())
		{
			continue;
		}

		// HACK:
		// Sometimes the last line is garbage.
		// See if this line is WAY too long
		// I'm assuming this is something to do with the parsting the stringstream object
		if (curStreetName.rawLineFromFile.length() > 1000)
		{
			continue;
		}

		std::vector<std::string> vecTokens;
		if (this->tokenizeLineWithQuotes(curStreetName.rawLineFromFile, vecTokens) > 0)
		{
			if (vecTokens.size() == 4)
			{
				// FullStreetName,StreetName,StreetType,PostDirection
				curStreetName.FullStreetName = vecTokens[0];
				curStreetName.StreetName = vecTokens[1];
				curStreetName.StreetType = vecTokens[2];
				curStreetName.PostDirection = vecTokens[3];

				this->vecStreetNames.push_back(curStreetName);
			}//if (vecTokens.size() == 4)
		}//if (this->tokenizeLineWithQuotes

	}//while (std::getline(...


	delete[] pRawStreetNamesFileData;
	return true;
}



// Randomly generates a person from the data
cPerson cPersonGenerator_Imp::generateRandomPerson(bool bBasedOnPopulationDistribution)
{
	cPerson newPerson;

	//      ___ _            _     __       _            _                            
	//     | __(_)_ _ ____ _| |_  / _|___  | |   __ _ __| |_   _ _  __ _ _ __  ___ ___
	//     | _|| | '_(_-< '_|  _| > _|_ _| | |__/ _` (_-<  _| | ' \/ _` | '  \/ -_|_-<
	//     |_| |_|_| /__/_|  \__| \_____|  |____\__,_/__/\__| |_||_\__,_|_|_|_\___/__/
	//                                                                                
	newPerson.first = this->getRandomFirstName(bBasedOnPopulationDistribution);
	newPerson.middle = this->getRandomFirstName(bBasedOnPopulationDistribution);
	newPerson.last = this->getRandomLastName(bBasedOnPopulationDistribution);


	//       ___             _         
	//      / __|___ _ _  __| |___ _ _ 
	//     | (_ / -_) ' \/ _` / -_) '_|
	//      \___\___|_||_\__,_\___|_|  
	//                                 
	// Gender breakdown in Canada as of 2022
	// https://www150.statcan.gc.ca/n1/daily-quotidien/220427/dq220427b-eng.htm and
	// https://www150.statcan.gc.ca/n1/daily-quotidien/250924/dq250924a-eng.htm and 
	// https://tradingeconomics.com/canada/population-female-percent-of-total-wb-data.html

	// "One in 300 people in Canada aged 15 and older are transgender or non-binary"
	// according to: https://www150.statcan.gc.ca/n1/daily-quotidien/220427/dq220427b-eng.htm
	if ((unsigned int)(this->p_theRandomizer->getNextRand()) % 300 == 0)
	{
		// Non-binary
		newPerson.gender = cPerson::eGenderType::NON_BINARY;
	}
	else
	{
		newPerson.gender = cPerson::eGenderType::FEMALE;

		// Of remaining 100%, 50.34% female and 49.66% male
		// ...or 4966 out of 10,000 people would be male
		unsigned long chanceMale = (this->p_theRandomizer->getNextRand()) % 10'000;
		if (chanceMale < 4966)
		{
			newPerson.gender = cPerson::eGenderType::MALE;
		}
	}

	// Do I report my gender?
	// According to: https://www.thelancet.com/journals/lanpub/article/PIIS2468-2667(23)00301-8/fulltext
	// between 0.2% and 5.6% in UK report, apparently
	//  So, let's say 2.5% change of NOT reporting
	// ...or 25 out of 1,000
	unsigned long chanceReporting = (this->p_theRandomizer->getNextRand()) % 1'000;
	if (chanceReporting < 25)
	{
		newPerson.gender = cPerson::eGenderType::RATHER_NOT_SAY_UNKNOWN;
	}

	//        _            
	//       /_\  __ _ ___ 
	//      / _ \/ _` / -_)
	//     /_/ \_\__, \___|
	//           |___/     
	// From 16 to 100
	newPerson.age = ((this->p_theRandomizer->getNextRand()) % 84) + 16;

	//        _      _    _               
	//       /_\  __| |__| |_ _ ___ ______
	//      / _ \/ _` / _` | '_/ -_|_-<_-<
	//     /_/ \_\__,_\__,_|_| \___/__/__/
	//                                    
	// From 1 to 10'000
	newPerson.streetNumber = (int)((this->p_theRandomizer->getNextRand()) % 10'000);
	unsigned int streetIndex = this->p_theRandomizer->getNextRand() % (this->vecStreetNames.size() - 1);
	newPerson.streetName = this->vecStreetNames[streetIndex].StreetName;
	newPerson.streetType = this->vecStreetNames[streetIndex].StreetType;
	newPerson.streetDirection = this->vecStreetNames[streetIndex].PostDirection;

	sCanadianCityInfo randomCity = getRandomCity(bBasedOnPopulationDistribution);
	newPerson.city = randomCity.name;
	newPerson.province = randomCity.province;

	//      ___        _        _    ___        _     
	//     | _ \___ __| |_ __ _| |  / __|___ __| |___ 
	//     |  _/ _ (_-<  _/ _` | | | (__/ _ Y _` / -_)
	//     |_| \___/__/\__\__,_|_|  \___\___|__,_\___|
	//                                                
	newPerson.postalCode[0] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 26) + 'A');
	newPerson.postalCode[1] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 10) + '0');
	newPerson.postalCode[2] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 26) + 'A');
	newPerson.postalCode[3] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 10) + '0');
	newPerson.postalCode[4] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 26) + 'A');
	newPerson.postalCode[5] = (char)((unsigned int)(this->p_theRandomizer->getNextRand() % 10) + '0');

	//      ___ _                                  _            
	//     | _ \ |_  ___ _ _  ___   _ _ _  _ _ __ | |__ ___ _ _ 
	//     |  _/ ' \/ _ \ ' \/ -_) | ' \ || | '  \| '_ Y -_) '_|
	//     |_| |_||_\___/_||_\___| |_||_\_,_|_|_|_|_.__|___|_|  
	//                          
	// How many phone numbers are we generating?
	// At least 1, but up to 4
	unsigned int numPhoneNumbers = (unsigned int)(this->p_theRandomizer->getNextRand() % 3) + 1;
	for (unsigned int phoneNumIndex = 0; phoneNumIndex != numPhoneNumbers; phoneNumIndex++)
	{
		newPerson.phoneNumbers[phoneNumIndex][0] = 1;	// Canada country code is '1'
		//
		// 1st digit should NOT be a zero (0)
		newPerson.phoneNumbers[phoneNumIndex][1] = (unsigned int)(this->p_theRandomizer->getNextRand() % 9) + 1;

		for (unsigned int digitIndex = 2; digitIndex != 11; digitIndex++)
		{
			// 0 to 9
			newPerson.phoneNumbers[phoneNumIndex][digitIndex] = (unsigned int)(this->p_theRandomizer->getNextRand() % 10);
		}
	}//for (...phoneNumIndex...

	//      ___ ___ _  _ 
	//     / __|_ _| \| |
	//     \__ \| || .` |
	//     |___/___|_|\_|
	//                   
	char cSIN[10] = { 0 };	// Clear to zeros
	cSIN[9] = '\0';
	for (unsigned int digitIndex = 0; digitIndex != 9; digitIndex++)
	{
		cSIN[digitIndex] = (unsigned int)(this->p_theRandomizer->getNextRand() % 10);
	}
	// Can start with a zero (0) but the 2nd digit +shoudln't+ be a zero
	// (to prevent a number like 0000xxxxx)
	if (cSIN[1] == 0)
	{
		// Pick another number between 1 and 9
		cSIN[1] = (unsigned int)(this->p_theRandomizer->getNextRand() % 9) + 1;
	}
	// Convert to string that atoi() can parse
	for ( unsigned int index = 0; index != 9; index++)
	{
		cSIN[index] += '0';
	}
	// Last digit is a zero, so will be accepted as a string
	newPerson.SIN = atoi(cSIN);

	return newPerson;
}



unsigned int cPersonGenerator_Imp::getNumberOfNamesLoaded(void)
{
	return (unsigned int)this->vecBabyNames.size();
}

unsigned int cPersonGenerator_Imp::getNumberOfSurnamesLoaded(void)
{
	return (unsigned int)this->vecSurnames.size();
}

unsigned int cPersonGenerator_Imp::getNumberOfStreetsLoaded(void)
{
	return (unsigned int)this->vecStreetNames.size();
}

std::string cPersonGenerator_Imp::getRandomFirstName(bool bUseDistribution)
{
	std::string firstName;

	if ( ! bUseDistribution ) 
	{
		// Just pick one
		unsigned int randNameIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % ((unsigned int)this->vecBabyNames.size() - 1) );

		firstName = this->vecBabyNames[randNameIndex].name;
	}
	else
	{
		unsigned int randNameIndex = this->p_theRandomizer->getNextRand() % this->totalBabyNameCount;

		// Pick the 1st baby (i.e. the most popular name)
		unsigned int pickedBabyIndex = 0;
		for (; pickedBabyIndex != (unsigned int)this->vecBabyNames.size(); pickedBabyIndex++)
		{
			if (this->vecBabyNames[pickedBabyIndex].cumulative_ranking >= randNameIndex)
			{
				// gone to far
				break;
			}
		}

		firstName = this->vecBabyNames[pickedBabyIndex].name;

	}//if ( ! bUseDistribution ) 

	return firstName;
}

std::string cPersonGenerator_Imp::getRandomLastName(bool bUseDistribution)
{
	std::string lastName;


	if ( ! bUseDistribution )
	{
		// Just pick one
		unsigned int randNameIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % ((unsigned int)this->vecSurnames.size() - 1) );

		lastName = this->vecSurnames[randNameIndex].name;
	}
	else
	{
		double randIndex = this->p_theRandomizer->getNextRand() % this->totalSurnameCount;
		double randNameIndex = randIndex/(double)this->totalSurnameCount;

		// Pick the 1st surname (i.e. the most popular name)
		unsigned int pickedSurnameIndex = 0;
		for (; pickedSurnameIndex != (unsigned int)this->vecSurnames.size(); pickedSurnameIndex++)
		{
			if (this->vecSurnames[pickedSurnameIndex].cumulative_proportion_as_percentage >= randNameIndex)
			{
				// gone to far
				break;
			}
		}

		lastName = this->vecSurnames[pickedSurnameIndex].name;
		// If the percentage picked is really high, then it'll 
		//	cloose "ALL OTHER NAMES".
		// If so, then pick "SMITH" (the most popular)
		if (lastName == "ALL OTHER NAMES")
		{
			lastName = "SMITH";
		}


	}//if ( ! bUseDistribution )

	return lastName;
}

sCanadianCityInfo cPersonGenerator_Imp::getRandomCity(bool bUseDistribution)
{
	sCanadianCityInfo city;

	if ( ! bUseDistribution )
	{
		// Just pick one
		unsigned int randCityIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % ((unsigned int)this->vecCanadianCities.size() - 1) );

		city = this->vecCanadianCities[randCityIndex];
	}
	else
	{
		unsigned long randomPopulation = this->p_theRandomizer->getNextRand() % this->totalCanadianCityPopulation;

		// Pick the 1st city
		unsigned int pickedCityIndex = 0;
		for (; pickedCityIndex != (unsigned int)this->vecCanadianCities.size(); pickedCityIndex++)
		{
			if (this->vecCanadianCities[pickedCityIndex].cumulative_population >= randomPopulation)
			{
				// gone to far
				break;
			}
		}

		city = this->vecCanadianCities[pickedCityIndex];
	}

	return city;
}

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
sCanadianCityInfo getRandomCityByDistribution(void);


std::string cPersonGenerator_Imp::eProvinceAsString(eProvince province)
{
	switch (province)
	{
	case MANITOBA:
		return "Manitoba";
	case NEW_BRUNSWICK:
		return "New Brunswick";
	case NEWFOUNDLAND_AND_LABRADOR:
		return "Newfoundland and Labrador";
	case ONTARIO:
		return "Ontario";
	case QUEBEC:
		return "Québec";
	case SASKATCHEWAN:
		return "Saskatchewan";
	case SANTAS_VILLAGE:
		return "North Pole";
	}//switch (province)

	// UNKNOWN_OR_UNDEFINED
	return "Unknown or undefined";
		
}