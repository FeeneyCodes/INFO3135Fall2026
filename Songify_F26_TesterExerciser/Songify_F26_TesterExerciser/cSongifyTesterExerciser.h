#ifndef _cSongifyTesterExerciser_HG_
#define _cSongifyTesterExerciser_HG_

#include <string>
#include <vector>
#include <map>
#include "cLogger.h"

#include "cMusicGenerator.h"
#include "cPersonGenerator.h"
#include "StudentSolutionFiles/cSongify.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

class cSongifyTesterExerciser
{
public:
	cSongifyTesterExerciser();
	~cSongifyTesterExerciser();

	struct sTestElement
	{
		std::string testFunctionName;
		unsigned int successCount = 0;
		unsigned int errorCount = 0;
		std::string anyDetails;
	};
	// This is a quick-n-dirty list of what the test results had.
	// This is listed as a summary at the end of the log
	std::map<std::string /*testFunctionName*/, sTestElement> m_MapTestElements;
	void generateTestsSummary(void);

	void startLog(void);
	std::string getTestPCInfo(void);

	// Returns false if it can't load the testing data
	//
	// Normally, this will use dozen people and 50 songs
	// If stress testing, 100s of users and 1000s of songs will be used 
	//	and a detailed summary will NOT be written to the log.
	bool initializeTestData(unsigned int maxNumUsers = 12,
		unsigned int maxNumSongs = 50);
	// This will only generate new test data, but not reload the generators
	// Note: it's just using rand() for random number generation, 
	//       so you might want to keep the maximums to less than 30K.
	bool regenerateTestData(unsigned int maxNumUsers = 12,
		unsigned int maxNumSongs = 50);

	void setStudentInfo(std::string studentInfo);

	void setSongifyInstance(cSongify* pStudentsSongify);

	// When using a large number of users, you might want
	//	to disable the verbose log output.
	// When true, all details (users, songs, etc.) are listed.
	// When false, only the summary is generated (OK, num errors, etc.).
	bool RunAllTests(bool bVerboseLogOutput = true);

	// These are separate tests, if needed.
	// RunAllTests() runs all of these, though
	bool RunBasicUserTests(bool bVerboseLogOutput = true);
	bool RunBasicSongTests(bool bVerboseLogOutput = true);
	bool RunPlaylistTests(bool bVerboseLogOutput = true);


	// Some methods used by the test methods
	bool compareUserLists(std::vector<cPerson>& vecPeopleReference, cPerson* pUserArray, unsigned int userArraySize,
		unsigned int& numUsersMissing, std::vector<std::string>& vecErrorDetails);
	bool compareUserListsInOrder(std::vector<cPerson>& vecPeopleReference, cPerson* pUserArray, unsigned int userArraySize,
		std::vector<std::string>& vecErrorDetails);
	// Returns true if identical
	bool comparePeople(cPerson& A, cPerson& B, std::vector<std::string>& vecDifferences, bool bIgnoreID = false);
	// Uses above, then ignores vecDifferences
	bool comparePeople(cPerson& A, cPerson& B, bool bIgnoreID = false);
	// Returns true if identical (for non-string things like postal code & phone number)
	bool compareCharBuffers(char* pBuffA, char* pBuffB, unsigned int size);
	// Helpful for a comparison print out
	void sortPeopleBySIN(std::vector<cPerson>& vecPeople);
	void sortPeopleBySongifyID(std::vector<cPerson>& vecPeople);
	void sortPeopleByName(std::vector<cPerson>& vecPeople, bool bAscending);

	// Writes to log (with timestamp)
	void Log(std::string text);

	// Doesn't compare the rating or number-of-times-played
	bool compareSong(cSong& A, cSong& B, std::vector<std::string>& vecDifferences, bool bIncludeSongID);
	// Can compare the ratings, etc. 
	bool compareSongInPlaylist(cSong& A, cSong& B, std::vector<std::string>& vecDifferences,
		bool bIgnoreRatings, bool bIgnorePlayCount);

private:
	cLogger* m_pLog = nullptr;
	std::string m_studentInfo;
	cSongify* m_pSongify = nullptr;

	bool m_bVerboseLogOutput = true;

	cMusicGenerator* m_pTheMusic = nullptr;
	cPersonGenerator* m_pThePeople = nullptr;

	cPerson* m_getRandom_pUser(void);
	// Returns NULL if can't find them
	cPerson* m_get_pUserByID(unsigned int songifyID);
	std::vector<cPerson> m_vecSomePeople;
	cSong* m_getRandom_pSong(void);
	cSong* m_findSongByID(unsigned int songID);
	std::vector<cSong> m_vecSomeSongs;

	// Picks values that aren't in the list of people
	unsigned int getInvalidSongifyID(void);
	unsigned int getInvalidSIN(void);

	unsigned int m_maxNumTestUsers = 12;
	unsigned int m_maxNumTestSongs = 50;

	// The playlists, by songify ID
	// This song is a copy of the main song, and holds the rankings
	std::map<unsigned int /*songifyID*/, std::vector<cSong> > m_mapUser_vecSongPlaylist;
	// Returns false if can't find user
	bool m_getTestUserPlaylist(unsigned int songifyID, std::vector<cSong>& vecSongPlaylist);
	// Returns false if no user has that many songs
	bool m_getRandTestUserPlaylistWithMinimumNumSongs(unsigned int& songifyID, std::vector<cSong>& vecSongPlaylist, unsigned int minNumberOfSongs);
	// The songs are saved as a copy, so this updates them if there's any song details update
	void m_updateTestUserPlaylistDataToMatchReferenceSongsList(void);

	// Returns true if identical (based on flags)
	// bIgnoreOrder = false --> ALL same songs but different order
	struct sPlaylistCompareParams
	{
		// false -> lists have to match exactly as passed (order as well)
		//          (so same songs but wrong order = NOT the same)
		bool bIgnoreOrder = false;
		bool bIgnoreRatings = true;
		bool bIgnorePlayCount = true;
		bool bIgnoreSongID = false;
	};
	bool m_comparePlaylists(std::vector<cSong> refPlaylist, std::vector<cSong> testPlaylist,
		std::vector<std::string>& vecDifferences, sPlaylistCompareParams comparisonSettings);

	typedef std::map<std::string, sTestElement>::iterator iterMapTest;
	void m_addTestResult(std::string testFunctionName);	// Adds initial value to map

	void m_testResult_Success(std::string testFunctionName, unsigned int successCountToAdd, std::string anyDetails);
	void m_testResult_Success(std::string testFunctionName);
	void m_testResult_Success(std::string testFunctionName, unsigned int successCountToAdd);
	void m_testResult_Success(std::string testFunctionName, std::string anyDetails);

	void m_testResult_Failure(std::string testFunctionName, unsigned int errorCountToAdd, std::string anyDetails);
	void m_testResult_Failure(std::string testFunctionName);
	void m_testResult_Failure(std::string testFunctionName, unsigned int errorCountToAdd);
	void m_testResult_Failure(std::string testFunctionName, std::string anyDetails);

};



#endif // cSongifyTesterExerciser
