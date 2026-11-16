#include "cSongifyTesterExerciser.h"
#include "formattingHelperFunctions.h"
#include <iomanip>		// for std::setprecision

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

cSongifyTesterExerciser::cSongifyTesterExerciser()
{
}


cSongifyTesterExerciser::~cSongifyTesterExerciser()
{
	if (!this->m_pLog)
	{
		this->m_pLog->Log("cSongifyTesterExerciser() instance ending.");
		this->m_pLog->Log("Closing log.");
		delete this->m_pLog;
	}
}

void cSongifyTesterExerciser::startLog(void)
{
	if ( ! this->m_pLog )
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_pLog->Log("Starting test log for student: " + this->m_studentInfo);
	this->m_pLog->Log("PC info:\n" + this->getTestPCInfo());
	return;
}


// Writes to log (with timestamp)
void cSongifyTesterExerciser::Log(std::string text)
{
	this->m_pLog->Log(text);
	return;
}

std::string cSongifyTesterExerciser::getTestPCInfo(void)
{
	std::stringstream ssPCInfo;
	// This reads the "This_PC_ConfigSummary.info" file from the C: drive
	std::ifstream fPCInfo("C:/This_PC_ConfigSummary.info");
	if ( fPCInfo.is_open() )
	{
		char buffer[1000] = {0};
		while ( fPCInfo.getline(buffer, 1000) )
		{
			ssPCInfo << "- " << buffer << '\n';
		}
	}
	else
	{
		ssPCInfo << "Can't read config file\n";
	}

	return ssPCInfo.str();
}

bool cSongifyTesterExerciser::initializeTestData(unsigned int maxNumUsers /*=12*/,
	unsigned int maxNumSongs /*=50*/)
{
	if (!this->m_pLog)
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_pThePeople = new cPersonGenerator();
	this->m_pTheMusic = new cMusicGenerator();

	std::string errorString;
	std::string babyNameFile = "Data/Popular Baby Names/names/yob2019.txt";
	std::string surnameFile = "Data/Surnames/names/Names_2010Census.csv";
	std::string streetNameFile = "Data/Street Names (San Francisco)/Street_Names.csv";

	this->m_pLog->Log("Loading the census data...");
	if (!this->m_pThePeople->LoadCensusFiles(babyNameFile, surnameFile, streetNameFile, errorString))
	{
		this->m_pLog->Log("Didn't load census files because: " + errorString);
		delete this->m_pThePeople;
		delete this->m_pTheMusic;
		return false;
	}
	this->m_pLog->Log("Census data loaded OK.");


	this->m_pLog->Log("Loading billboard music files...");
	std::string billboardFileName = "Data/billboard/kcmillersean-billboard-hot-100-1958-2017/data/audiio.csv";
	if (!this->m_pTheMusic->LoadMusicInformation(billboardFileName, errorString))
	{
		this->m_pLog->Log("Didn't load music information because: " + errorString);
		delete this->m_pThePeople;
		delete this->m_pTheMusic;
		return false;
	}
	this->m_pLog->Log("Billboard music file loaded OK.");

	// Data files loaded
	return this->regenerateTestData(maxNumUsers, maxNumSongs);
}

bool cSongifyTesterExerciser::regenerateTestData(unsigned int maxNumUsers /*=12*/,
	unsigned int maxNumSongs /*=50*/)
{

	if ((!this->m_pTheMusic) || (!this->m_pThePeople))
	{
		this->m_pLog->Log("regenerateTestData() called without calling initializeTestData() first.");
		return false;
	}

	this->m_vecSomePeople.clear();
	this->m_vecSomeSongs.clear();
	this->m_mapUser_vecSongPlaylist.clear();

	this->m_maxNumTestUsers = maxNumUsers;
	this->m_maxNumTestSongs = maxNumSongs;


	{// STARTOF: Generate test users
		std::stringstream ssLog;
		ssLog << "Generating " << maxNumUsers << " users";

		for (unsigned int count = 0; count != this->m_maxNumTestUsers; count++)
		{
			this->m_vecSomePeople.push_back(this->m_pThePeople->generateRandomPerson());
		}

		this->m_pLog->Log(ssLog.str());
	}// ENDOF: Generate test users

	{// STARTOF: Generate test songs
		std::stringstream ssLog;
		ssLog << "Generating " << maxNumSongs << " songs";
		this->m_pLog->Log(ssLog.str());

		unsigned int songsAdded = 0;
		// If we don't load the "hot_stuff" file as well, then the release dates are random,
		//	meaning we could have the identical song with different release dates.
		unsigned int duplicateSongsFound = 0;
		do
		{
			cSong s = this->m_pTheMusic->getRandomSong();

			// Functor for search
			class cSameSongID
			{
			public:
				unsigned int m_songID = 0;
				cSameSongID(unsigned int songID)
				{
					this->m_songID = songID;
				}
				bool operator()(cSong& A)
				{
					return (this->m_songID == A.getUniqueID());
				}
			};

			if (std::find_if(this->m_vecSomeSongs.begin(), this->m_vecSomeSongs.end(), cSameSongID(s.getUniqueID())) == this->m_vecSomeSongs.end())
			{
				// Song is unique, so add it
				this->m_vecSomeSongs.push_back(s);
				songsAdded++;
			}
			else
			{
				duplicateSongsFound++;
			}
		} while (songsAdded < this->m_maxNumTestSongs);


	}// ENDOF: Generate test songs

	{// STARTOF: Generate some playlists
		this->m_pLog->Log("Generating playlists");

		// Use up to a quarter (25%) of the max songs for each user
		for (unsigned int index = 0; index != this->m_vecSomePeople.size(); index++)
		{
			// Add an empty playlist
			unsigned int songifyID = this->m_vecSomePeople[index].getSongifyUniqueUserID();
			this->m_mapUser_vecSongPlaylist[songifyID] = std::vector<cSong>();

			// Get iterator to playlist
			std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itPL = this->m_mapUser_vecSongPlaylist.find(songifyID);

			// How many songs for this user
			int numSongs = rand() % (this->m_maxNumTestSongs / 4);

			std::vector<cSong*> vec_pSongs;
			for (unsigned int songCount = 0; songCount != numSongs; songCount++)
			{
				bool bSongAddedOK = true;
				do
				{
					bSongAddedOK = false;
					cSong* pRandSong = this->m_getRandom_pSong();

					// Have this one already?
					class cSongIDEqual
					{
					public:
						unsigned int m_SongID = 0;
						cSongIDEqual(unsigned int songID)
						{
							this->m_SongID = songID;
						}
						bool operator()(cSong& S)
						{
							return (this->m_SongID == S.getUniqueID());
						}
					};
					std::vector<cSong>::iterator itPLSong
						= std::find_if(itPL->second.begin(), itPL->second.end(), cSongIDEqual(pRandSong->getUniqueID()));

					if (itPLSong == itPL->second.end())
					{
						// Not on list, so copy it to playlist
						itPL->second.push_back(*pRandSong);
						bSongAddedOK = true;
					}

				} while (!bSongAddedOK);

			}//for ( unsigned int songCount

		}//for ( unsigned int index

	}// ENDOF: Generate test playlists

	return true;
}



void cSongifyTesterExerciser::setStudentInfo(std::string studentInfo)
{
	this->m_studentInfo = studentInfo;
	return;
}

void cSongifyTesterExerciser::setSongifyInstance(cSongify* pStudentsSongify)
{
	this->m_pSongify = pStudentsSongify;
	return;
}

bool cSongifyTesterExerciser::RunAllTests(bool bVerboseLogOutput /*=true*/)
{
	if (!this->m_pLog)
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_bVerboseLogOutput = bVerboseLogOutput;

	this->m_pLog->Log("Running all tests...");


	if (!this->RunBasicUserTests(bVerboseLogOutput))
	{
		this->m_pLog->Log("RunBasicUserTests() returned false.");
	}

	if (!RunBasicSongTests(bVerboseLogOutput))
	{
		this->m_pLog->Log("RunBasicSongTest() returned false.");
	}

	if (!RunPlaylistTests(bVerboseLogOutput))
	{
		this->m_pLog->Log("RunPlaylistTests() returned false.");
	}

	this->generateTestsSummary();

	this->m_pLog->Log("All tests completed.");

	return true;
}

// Returns true if identical (for non-string things like postal code & phone number)
bool cSongifyTesterExerciser::compareCharBuffers(char* pBuffA, char* pBuffB, unsigned int size)
{
	for (unsigned int index = 0; index < size; index++)
	{
		if (pBuffA[index] != pBuffB[index])
		{
			return false;
		}
	}
	// No errors
	return true;
}

// Adds initial value to map
void cSongifyTesterExerciser::m_addTestResult(std::string testFunctionName)
{
	// Function present (i.e. to update?)
	iterMapTest itTFN = this->m_MapTestElements.find(testFunctionName);
	if (itTFN != this->m_MapTestElements.end())
	{
		// Already there
		return;
	}

	sTestElement te;
	te.testFunctionName = testFunctionName;
	this->m_MapTestElements[testFunctionName] = te;
	return;
}


void cSongifyTesterExerciser::m_testResult_Success(std::string testFunctionName, unsigned int successCountToAdd, std::string anyDetails)
{
	// Function present (i.e. to update?)
	iterMapTest itTFN = this->m_MapTestElements.find(testFunctionName);
	if (itTFN != this->m_MapTestElements.end())
	{
		// Already there
		itTFN->second.successCount += successCountToAdd;
		if (!anyDetails.empty())
		{
			if (!itTFN->second.anyDetails.empty())
			{
				itTFN->second.anyDetails += "; ";
			}
			itTFN->second.anyDetails += anyDetails;
		}
		return;
	}

	sTestElement te;
	te.testFunctionName = testFunctionName;
	te.successCount += successCountToAdd;
	te.anyDetails = anyDetails;

	this->m_MapTestElements[testFunctionName] = te;
	return;
}

void cSongifyTesterExerciser::m_testResult_Success(std::string testFunctionName, unsigned int successCountToAdd)
{
	this->m_testResult_Success(testFunctionName, successCountToAdd, "");
	return;
}

void cSongifyTesterExerciser::m_testResult_Success(std::string testFunctionName, std::string anyDetails)
{
	this->m_testResult_Success(testFunctionName, 1, anyDetails);
	return;
}

void cSongifyTesterExerciser::m_testResult_Success(std::string testFunctionName)
{
	this->m_testResult_Success(testFunctionName, 1, "");
	return;
}


void cSongifyTesterExerciser::m_testResult_Failure(std::string testFunctionName, unsigned int errorCountToAdd, std::string anyDetails)
{
	// Function present (i.e. to update?)
	iterMapTest itTFN = this->m_MapTestElements.find(testFunctionName);
	if (itTFN != this->m_MapTestElements.end())
	{
		// Already there
		itTFN->second.errorCount += errorCountToAdd;
		if (!anyDetails.empty())
		{
			if (!itTFN->second.anyDetails.empty())
			{
				itTFN->second.anyDetails += "; ";
			}
			itTFN->second.anyDetails += anyDetails;
		}
		return;
	}

	// Didn't find it, so add it
	sTestElement te;
	te.testFunctionName = testFunctionName;
	te.errorCount = errorCountToAdd;
	te.anyDetails = anyDetails;
	this->m_MapTestElements[testFunctionName] = te;
	return;
}

void cSongifyTesterExerciser::m_testResult_Failure(std::string testFunctionName, unsigned int errorCountToAdd)
{
	this->m_testResult_Failure(testFunctionName, errorCountToAdd, "");
	return;
}

void cSongifyTesterExerciser::m_testResult_Failure(std::string testFunctionName, std::string anyDetails)
{
	this->m_testResult_Failure(testFunctionName, 1, anyDetails);
	return;
}

void cSongifyTesterExerciser::m_testResult_Failure(std::string testFunctionName)
{
	this->m_testResult_Failure(testFunctionName, 1, "");
	return;
}



cPerson* cSongifyTesterExerciser::m_getRandom_pUser(void)
{
	unsigned int randIndex = rand() % (int)(this->m_vecSomePeople.size() - 1);

	return &(this->m_vecSomePeople[randIndex]);
}

cSong* cSongifyTesterExerciser::m_getRandom_pSong(void)
{
	unsigned int randIndex = rand() % (int)(this->m_vecSomeSongs.size() - 1);

	return &(this->m_vecSomeSongs[randIndex]);
}

cSong* cSongifyTesterExerciser::m_findSongByID(unsigned int songID)
{
	for (unsigned int index = 0; index != this->m_vecSomeSongs.size(); index++)
	{
		if (this->m_vecSomeSongs[index].getUniqueID() == songID)
		{
			// Found it
			return &(this->m_vecSomeSongs[index]);
		}
	}
	// Didn't find match
	return nullptr;
}


// Returns NULL if can't find them
cPerson* cSongifyTesterExerciser::m_get_pUserByID(unsigned int songifyID)
{
	for (unsigned int index = 0; index != this->m_vecSomePeople.size(); index++)
	{
		if (songifyID == this->m_vecSomePeople[index].getSongifyUniqueUserID())
		{
			// Found 'em
			return &(this->m_vecSomePeople[index]);
		}
	}
	// Didn't find them
	return nullptr;
}


void cSongifyTesterExerciser::generateTestsSummary(void)
{
	std::string testSummaryFileName = this->m_pLog->getLogFileFullName();
	// Strip off the ".log"
	testSummaryFileName = testSummaryFileName.substr(0, testSummaryFileName.length() - 4);
	testSummaryFileName += ".csv";

	std::ofstream testSummaryCSV(testSummaryFileName);
	if (!testSummaryCSV.is_open())
	{
		this->m_pLog->appendText("Can't open [");
		this->m_pLog->appendText(testSummaryFileName);
		this->m_pLog->appendText("] test summary file.");
		this->m_pLog->Log_OutputAppendedText();
		return;
	}

	this->m_pLog->appendText("Test summary file [");
	this->m_pLog->appendText(testSummaryFileName);
	this->m_pLog->appendText("] opened.");
	this->m_pLog->Log_OutputAppendedText();

	//		struct sTestElement
	//	{
	//		std::string testFunctionName;
	//		unsigned int successCount = 0;
	//		unsigned int errorCount = 0;
	//		std::string anyDetails;
	//	};

	this->m_pLog->Log("Generating summary file.");

	testSummaryCSV
		<< this->m_studentInfo
		<< ", , , ,"
		<< std::endl;

	testSummaryCSV
		<< "Test Function Name" << ", "
		<< "Success Count" << ", "
		<< "Error Count" << ", "
		<< "Additional Details" << ", "
		<< "Test total" << ", "
		<< "Success %" << ", "
		<< std::endl;

	for (iterMapTest itTI = this->m_MapTestElements.begin(); itTI != this->m_MapTestElements.end(); itTI++)
	{
		sTestElement& te = itTI->second;

		unsigned int totalTests = te.successCount + te.errorCount;
		float successRatioPercent = ((float)te.successCount / (float)totalTests) * 100.0f;

		testSummaryCSV
			<< te.testFunctionName << ", "
			<< te.successCount << ", "
			<< te.errorCount << ", "
			<< te.anyDetails << ", "
			<< totalTests << ", "
			// For some reason setprecision(1) shows 2 decimals... 
			<< std::fixed << std::setprecision(1) << successRatioPercent << "%, "
			<< std::endl;
	}//for ( iterMapTest itTI...

	testSummaryCSV.close();

	this->m_pLog->Log("Done.");

	return;
}
