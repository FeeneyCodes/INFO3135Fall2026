#include "cSongifyTesterExerciser.h"
#include "formattingHelperFunctions.h"
#include <algorithm>
#include <assert.h>
#if _MSVC_LANG >= 201703L			//__cplusplus
#include <random>
#include <chrono>
#endif

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//



// Compare in order for differences
// This is for std::sort()
class cSortFunctorSongsIsEqual
{
public:
	bool m_bIgnoreRatings = false;
	bool m_bIgnorePlayCount = false;
	bool m_bIgnoreSongID = false;
	cSortFunctorSongsIsEqual(bool bIgnoreRatings, bool bIgnorePlayCount, bool bIgnoreSongID)
	{
		this->m_bIgnoreRatings = bIgnoreRatings;
		this->m_bIgnorePlayCount = bIgnorePlayCount;
		this->m_bIgnoreSongID = bIgnoreSongID;
	}
	bool operator()(cSong& A, cSong& B)
	{
		return this->areEqual(A, B);
	}

	bool areEqual(cSong& A, cSong& B)
	{
		if (A.name != B.name) { return false; }
		if (A.artist != B.artist) { return false; }
		if (A.releaseDateYear != B.releaseDateYear) { return false; }
		if (A.releaseDateMonth != B.releaseDateMonth) { return false; }
		if (A.releaseDateDay != B.releaseDateDay) { return false; }

		if (!this->m_bIgnorePlayCount)
		{
			if (A.numberOfTimesPlayed != B.numberOfTimesPlayed)
			{
				return false;
			}
		}
		if (!this->m_bIgnoreRatings)
		{
			if (A.rating != B.rating)
			{
				return false;
			}
		}
		if (!this->m_bIgnoreSongID)
		{
			if (A.getUniqueID() != B.getUniqueID())
			{
				return false;
			}
		}
		// They are the same
		return true;
	}//bool operator()
};//class cSongsIsEqual



bool cSongifyTesterExerciser::RunPlaylistTests(bool bVerboseLogOutput /*=true*/)
{
	if (!this->m_pLog)
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_bVerboseLogOutput = bVerboseLogOutput;

	this->m_pLog->LogPrintLineBreak(60);
	this->m_pLog->Log("RunPlaylistTests() starting.");
	if (!this->m_bVerboseLogOutput)
	{
		this->m_pLog->Log("(Generating limited/non-verbose output)");
	}
	this->m_pLog->LogPrintLineBreak(60);

	this->m_updateTestUserPlaylistDataToMatchReferenceSongsList();

	{// Testing AddSongToUserLibrary
		this->m_pLog->Log("Testing AddSongToUserLibrary()");
		this->m_addTestResult("AddSongToUserLibrary()");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Add all the songs to all the playlists
		unsigned int brief_AddSongToUserLibrary_CorrectCount = 0;
		unsigned int brief_AddSongToUserLibrary_ErrorCount = 0;
		unsigned int brief_Playlist_CorrectCount = 0;
		unsigned int brief_Playlist_ErrorCount = 0;
		unsigned int brief_GetUsersSongLibrary_CorrectCount = 0;
		unsigned int brief_GetUsersSongLibrary_ErrorCount = 0;

		for (cPerson p : this->m_vecSomePeople)
		{
			std::vector<cSong> vecReferenceUserSongPlaylist;
			if (this->m_getTestUserPlaylist(p.getSongifyUniqueUserID(), vecReferenceUserSongPlaylist))
			{
				// Test user has songs on playlist
				for (cSong s : vecReferenceUserSongPlaylist)
				{
					std::string errorString;
					if (this->m_pSongify->AddSongToUserLibrary(p.getSongifyUniqueUserID(), s.getUniqueID(), errorString))
					{
						brief_AddSongToUserLibrary_CorrectCount++;
						if (this->m_bVerboseLogOutput)
						{
							this->m_pLog->Log("(CORRECT) AddSongToUserLibrary() returned true adding a song");
						}
						this->m_testResult_Success("AddSongToUserLibrary()");
					}
					else
					{
						brief_AddSongToUserLibrary_CorrectCount++;
						if (this->m_bVerboseLogOutput)
						{
							this->m_pLog->Log("Error: AddSongToUserLibrary() returned false adding a song (should return true)");
						}
						this->m_testResult_Failure("AddSongToUserLibrary()");
					}
				}//for ( cSong s

				// See if the songs were added
				cSong* pSongPlaylist = nullptr;
				unsigned int numItemsInPlaylist = 0;
				this->m_pSongify->GetUsersSongLibrary(p.getSongifyUniqueUserID(), pSongPlaylist, numItemsInPlaylist);

				if (numItemsInPlaylist != vecReferenceUserSongPlaylist.size())
				{
					brief_Playlist_ErrorCount++;
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("Error: GetUsersSongLibrary() returned incorrect playlist size");
					}
					this->m_testResult_Failure("GetUsersSongLibrary()");
				}
				// Copy to vector for later comparison
				std::vector<cSong> vecTestUserPlaylist;
				for (unsigned int index = 0; index < numItemsInPlaylist; index++)
				{
					vecTestUserPlaylist.push_back(pSongPlaylist[index]);
				}
				std::vector<std::string> vecDifferences;
				sPlaylistCompareParams comparisonSettings;
				comparisonSettings.bIgnoreOrder = true;
				comparisonSettings.bIgnorePlayCount = true;
				comparisonSettings.bIgnoreRatings = true;
				if (this->m_comparePlaylists(vecReferenceUserSongPlaylist, vecTestUserPlaylist, vecDifferences, comparisonSettings))
				{
					brief_Playlist_CorrectCount++;
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("(CORRECT) AddSongToUserLibrary(): GetUsersSongLibrary() returned identical playlist");
					}
					this->m_testResult_Success("AddSongToUserLibrary()");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}
				else
				{
					// Playlists don't match
					brief_Playlist_ErrorCount++;
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("Error: AddSongToUserLibrary(): GetUsersSongLibrary() returned incorrect playlist");
						this->m_pLog->appendText("  There were ");
						this->m_pLog->appendNumber(vecDifferences.size());
						this->m_pLog->appendText(" differences between playlists");
						this->m_pLog->Log_OutputAppendedText();
					}
					this->m_testResult_Failure("AddSongToUserLibrary()");
					this->m_testResult_Failure("GetUsersSongLibrary()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("Here are the differences in the playlists:");
						for (std::string line : vecDifferences)
						{
							this->m_pLog->Log(" - " + line);
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(

			}//if ( this->m_getTestUerPlaylist(
		}//for ( cPerson p

	}// Testing AddSongToUserLibrary




	this->m_pLog->LogPrintLineBreak(60);

	{// Testing RemoveSongFromUserLibrary
		this->m_pLog->Log("Testing RemoveSongFromUserLibrary()");
		this->m_addTestResult("RemoveSongFromUserLibrary()");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}


		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test RemoveSongFromUserLibrary()");
				this->m_testResult_Failure("RemoveSongFromUserLibrary()", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Remove the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
				// Move to 2nd song (index 1)
				itSong++;

				this->m_pLog->Log("Attempting to remove this song: " + printableSong(*itSong));
				this->m_pLog->Log("From user: " + printablePerson(*pRandomPerson, true));

				// Save removed song ID
				unsigned int songIDToRemove = itSong->getUniqueID();
				// Remove song from reference user's playlist (for later comparison)
				itRandUserPlaylist->second.erase(itSong);
				// TODO: Verify that this song IS removed from the reference user's playlist

				std::string errorString;
				if (this->m_pSongify->RemoveSongFromUserLibrary(theRandomPerson.getSongifyUniqueUserID(), songIDToRemove, errorString))
				{
					// CORRECT - returned true
					this->m_pLog->Log("(CORRECT) RemoveSongFromUserLibrary() returned true");
					this->m_testResult_Success("RemoveSongFromUserLibrary()");
				}
				else
				{
					// Error - should have removed true
					this->m_pLog->Log("Error: RemoveSongFromUserLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("RemoveSongFromUserLibrary()");
				}

				// Get updated playlist and compare to see if song IS removed
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibrary(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibrary()");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}
				sPlaylistCompareParams compareSettings;
				compareSettings.bIgnoreOrder = true;
				compareSettings.bIgnorePlayCount = true;
				compareSettings.bIgnoreRatings = true;
				std::vector<std::string> vecDifferences;
				if (this->m_comparePlaylists(itRandUserPlaylist->second, testPlayList, vecDifferences, compareSettings))
				{
					this->m_pLog->Log("(CORRECT) Playlists are the same (after playlist song delete)");
					this->m_testResult_Success("RemoveSongFromUserLibrary()");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}
				else
				{
					this->m_pLog->Log("Error: Playlist are not the same (after playlist song delete)");
					this->m_testResult_Failure("RemoveSongFromUserLibrary()");
					this->m_testResult_Failure("GetUsersSongLibrary()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("They had the following differences:");
						for (std::string line : vecDiffences)
						{
							this->m_pLog->Log(" - " + line);
						}
						this->m_pLog->Log("Reference playlist: ");
						for (cSong s : itRandUserPlaylist->second)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
						this->m_pLog->Log("Test playlist: ");
						for (cSong s : testPlayList)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(...
			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test RemoveSongFromUserLibrary()");
			this->m_testResult_Failure("RemoveSongFromUserLibrary()", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing RemoveSongFromUserLibrary


	this->m_pLog->LogPrintLineBreak(60);

	//	this->m_updateTestUserPlaylistDataToMatchSomeSongsList();

	{// Testing UpdateSong() relating to playlists
		this->m_pLog->Log("Testing UpdateSong() relating to playlists");
		this->m_addTestResult("UpdateSong():PlayList");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}

		// Find this user in Songify
		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
				this->m_testResult_Failure("UpdateSong():PlayList", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Update the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
				// Move to 2nd song (index 1)
				itSong++;

				this->m_pLog->Log("Attempting to update this song: " + printableSong(*itSong));
				this->m_pLog->Log("From user: " + printablePerson(*pRandomPerson, true));

				cSong oldSongData = *itSong;
				// Get new date for updated song
				cSong newSong = this->m_pTheMusic->getRandomSong();

				// Update data on reference song
				cSong* pReferenceSong = this->m_findSongByID(itSong->getUniqueID());

				pReferenceSong->name = newSong.name;
				pReferenceSong->artist = newSong.artist;
				pReferenceSong->releaseDateYear = newSong.releaseDateYear;
				pReferenceSong->releaseDateMonth = newSong.releaseDateMonth;
				pReferenceSong->releaseDateDay = newSong.releaseDateDay;

				this->m_pLog->Log("Song updated to: " + printableSong(*pReferenceSong));

				// Update all the reference songlists
				this->m_updateTestUserPlaylistDataToMatchReferenceSongsList();

				std::string errorString;
				if (this->m_pSongify->UpdateSong(*pReferenceSong, errorString))
				{
					// CORRECT - returned true
					this->m_pLog->Log("(CORRECT) UpdateSong() returned true");
					this->m_testResult_Success("UpdateSong():PlayList");
				}
				else
				{
					// Error - should have returned true
					this->m_pLog->Log("Error: UpdateSong() returned false (should have returned true)");
					this->m_testResult_Failure("UpdateSong():PlayList");
				}

				// Get updated playlist and compare to see if song IS updated
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibrary(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibrary()", "UpdateSong():PlayList");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibrary()", "UpdateSong():PlayList");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}
				sPlaylistCompareParams compareSettings;
				compareSettings.bIgnoreOrder = true;
				compareSettings.bIgnorePlayCount = true;
				compareSettings.bIgnoreRatings = true;
				std::vector<std::string> vecDifferences;
				if (this->m_comparePlaylists(itRandUserPlaylist->second, testPlayList, vecDifferences, compareSettings))
				{
					this->m_pLog->Log("(CORRECT) Playlists are the same (after song update)");
					this->m_testResult_Success("UpdateSong():PlayList");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}
				else
				{
					this->m_pLog->Log("Error: Playlist are not the same (after song update)");
					this->m_testResult_Failure("UpdateSong():PlayList");
					this->m_testResult_Failure("GetUsersSongLibrary()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("They had the following differences:");
						for (std::string line : vecDiffences)
						{
							this->m_pLog->Log(" - " + line);
						}
						this->m_pLog->Log("Reference playlist: ");
						for (cSong s : itRandUserPlaylist->second)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
						this->m_pLog->Log("Test playlist: ");
						for (cSong s : testPlayList)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(...
			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
			this->m_testResult_Failure("UpdateSong():PlayList", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing UpdateSong() relating to playlists


	this->m_pLog->LogPrintLineBreak(60);

	//	this->m_updateTestUserPlaylistDataToMatchSomeSongsList();

	{// Testing DeleteSong() relating to playlists
		this->m_pLog->Log("Testing DeleteSong() relating to playlists");
		this->m_addTestResult("DeleteSong():PlayList");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}

		// Find this user in Songify
		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
				this->m_testResult_Failure("DeleteSong():PlayList", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Update the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
				// Move to 2nd song (index 1)
				itSong++;

				this->m_pLog->Log("Attempting to delete this song (from Songify): " + printableSong(*itSong));

				cSong songToDeleteInfo = *itSong;

				for (std::vector<cSong>::iterator itRefSong = this->m_vecSomeSongs.begin(); itRefSong != this->m_vecSomeSongs.end(); itRefSong++)
				{
					if (itRefSong->getUniqueID() == songToDeleteInfo.getUniqueID())
					{
						// This is the song. Delete it.
						this->m_vecSomeSongs.erase(itRefSong);
						// Get out of here (iterator will be screwed up, anyway)
						break;
					}
				}

				// Update all the reference songlists
				this->m_updateTestUserPlaylistDataToMatchReferenceSongsList();

				std::string errorString;
				if (this->m_pSongify->DeleteSong(songToDeleteInfo.getUniqueID(), errorString))
				{
					// CORRECT - returned true
					this->m_pLog->Log("(CORRECT) DeleteSong() returned true");
					this->m_testResult_Success("DeleteSong():PlayList");
				}
				else
				{
					// Error - should have returned true
					this->m_pLog->Log("Error: UpdateDeleteSongSong() returned false (should have returned true)");
					this->m_testResult_Failure("DeleteSong():PlayList");
				}

				// Get updated playlist and compare to see if song IS updated
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibrary(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibrary()", "DeleteSong():PlayList");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibrary()", "DeleteSong():PlayList");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}

				// Retrieve the reference user's playlist again (after the delete)
				itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				sPlaylistCompareParams compareSettings;
				compareSettings.bIgnoreOrder = true;
				compareSettings.bIgnorePlayCount = true;
				compareSettings.bIgnoreRatings = true;
				std::vector<std::string> vecDifferences;
				if (this->m_comparePlaylists(itRandUserPlaylist->second, testPlayList, vecDifferences, compareSettings))
				{
					this->m_pLog->Log("(CORRECT) Playlists are the same (after song update)");
					this->m_testResult_Success("DeleteSong():PlayList");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}
				else
				{
					this->m_pLog->Log("Error: Playlist are not the same (after song update)");
					this->m_testResult_Failure("UpdateSong():PlayList");
					this->m_testResult_Failure("GetUsersSongLibrary()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("They had the following differences:");
						for (std::string line : vecDiffences)
						{
							this->m_pLog->Log(" - " + line);
						}
						this->m_pLog->Log("Reference playlist: ");
						for (cSong s : itRandUserPlaylist->second)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
						this->m_pLog->Log("Test playlist: ");
						for (cSong s : testPlayList)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(...
			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test DeleteSong()");
			this->m_testResult_Failure("DeleteSong():PlayList", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing DeleteSong() relating to playlists


	this->m_pLog->LogPrintLineBreak(60);


	{// Testing UpdateRatingOnSong() 
		this->m_pLog->Log("Testing UpdateRatingOnSong()");
		this->m_addTestResult("UpdateRatingOnSong()");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}

		// Find this user in Songify
		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
				this->m_testResult_Failure("UpdateRatingOnSong()", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Update the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				// Save the currentRatings
				std::vector< std::pair<unsigned int, unsigned int> > vecSongID_OldRatings;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					vecSongID_OldRatings.push_back(std::pair<unsigned int, unsigned int>(itSong->getUniqueID(), itSong->rating));
				}

				// Update the ratings
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					// (rating, by the USER, from 0 to 5)
					unsigned int newRating = rand() % 6;
					itSong->rating = newRating;
				}

				this->m_pLog->appendText("Updating ");
				this->m_pLog->appendNumber(itRandUserPlaylist->second.size());
				this->m_pLog->appendText(" song ratings in playlist");
				this->m_pLog->Log_OutputAppendedText();

				// Update the ratings to match
				unsigned int numErrors = 0;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					if (!this->m_pSongify->UpdateRatingOnSong(foundPerson.getSongifyUniqueUserID(), itSong->getUniqueID(), itSong->rating))
					{
						numErrors++;
					}
				}//for ( std::vector<cSong>::iterator...

				// How many worked?
				if (numErrors == 0)
				{
					this->m_pLog->Log("(CORRECT) UpdateRatingOnSong() returned true every time");
					this->m_testResult_Success("UpdateRatingOnSong()");
				}
				else
				{
					this->m_pLog->appendText("Error: UpdateRatingOnSong() returned false ");
					this->m_pLog->appendNumber(numErrors);
					this->m_pLog->appendText(" times (should have returned true)");
					this->m_testResult_Failure("UpdateRatingOnSong()");
				}

				// Get updated playlist and compare to see if song ratings match
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibrary(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibrary()", "UpdateRatingOnSong()");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibrary()", "UpdateRatingOnSong()");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}

				// Retrieve the reference user's playlist again (after the delete)
				itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				sPlaylistCompareParams compareSettings;
				compareSettings.bIgnoreOrder = true;
				compareSettings.bIgnorePlayCount = true;
				compareSettings.bIgnoreRatings = false;		// <---- *****
				std::vector<std::string> vecDifferences;
				if (this->m_comparePlaylists(itRandUserPlaylist->second, testPlayList, vecDifferences, compareSettings))
				{
					this->m_pLog->Log("(CORRECT) Playlists are the same (after song update)");
					this->m_testResult_Success("UpdateRatingOnSong()");
					this->m_testResult_Success("GetUsersSongLibrary()");
				}
				else
				{
					this->m_pLog->Log("Error: Playlist are not the same (after song update)");
					this->m_testResult_Failure("UpdateRatingOnSong()");
					this->m_testResult_Failure("GetUsersSongLibrary()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("They had the following differences:");
						for (std::string line : vecDiffences)
						{
							this->m_pLog->Log(" - " + line);
						}
						this->m_pLog->Log("Reference playlist: ");
						for (cSong s : itRandUserPlaylist->second)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
						this->m_pLog->Log("Test playlist: ");
						for (cSong s : testPlayList)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(...
			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test UpdateRatingOnSong()");
			this->m_testResult_Failure("UpdateRatingOnSong()", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing UpdateRatingOnSong() 


	this->m_pLog->LogPrintLineBreak(60);


	{// Testing GetCurrentSongRating() 
		this->m_pLog->Log("Testing UpdateRatingOnSong()");
		this->m_addTestResult("GetCurrentSongRating()");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}

		// Find this user in Songify
		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
				this->m_testResult_Failure("GetCurrentSongRating()", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Update the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				// Save the currentRatings
				std::vector< std::pair<unsigned int, unsigned int> > vecSongID_OldRatings;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					vecSongID_OldRatings.push_back(std::pair<unsigned int, unsigned int>(itSong->getUniqueID(), itSong->rating));
				}

				// Update the ratings
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					// (rating, by the USER, from 0 to 5)
					unsigned int newRating = rand() % 6;
					itSong->rating = newRating;
				}

				this->m_pLog->appendText("Updating ");
				this->m_pLog->appendNumber(itRandUserPlaylist->second.size());
				this->m_pLog->appendText(" song ratings in playlist");
				this->m_pLog->Log_OutputAppendedText();

				// Update the ratings to match
				unsigned int numErrors = 0;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					if (!this->m_pSongify->UpdateRatingOnSong(foundPerson.getSongifyUniqueUserID(), itSong->getUniqueID(), itSong->rating))
					{
						numErrors++;
					}
				}//for ( std::vector<cSong>::iterator...

				// How many worked?
				if (numErrors == 0)
				{
					this->m_pLog->Log("(CORRECT) UpdateRatingOnSong() returned true every time");
					this->m_testResult_Success("UpdateRatingOnSong()");
				}
				else
				{
					this->m_pLog->appendText("Error: UpdateRatingOnSong() returned false ");
					this->m_pLog->appendNumber(numErrors);
					this->m_pLog->appendText(" times (should have returned true)");
					this->m_testResult_Failure("UpdateRatingOnSong()");
				}

				// Get updated playlist and compare to see if song ratings match
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibrary(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibrary()", "UpdateRatingOnSong()");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibrary()", "UpdateRatingOnSong()");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}

				// Retrieve the reference user's playlist again
				itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				this->m_pLog->Log("Checking rating on this song: " + printableSong(itRandUserPlaylist->second[0]));

				unsigned int testSongRanking = 0;
				if (this->m_pSongify->GetCurrentSongRating(randUserSongifyID, itRandUserPlaylist->second[0].getUniqueID(), testSongRanking))
				{
					this->m_pLog->Log("(CORRECT) GetCurrentSongRating() returned true");
					this->m_testResult_Success("GetCurrentSongRating()");
				}
				else
				{
					this->m_pLog->Log("Error: GetCurrentSongRating() returned true");
					this->m_testResult_Failure("GetCurrentSongRating()");
				}

				if (itRandUserPlaylist->second[0].rating == testSongRanking)
				{
					this->m_pLog->appendText("(CORRECT) GetCurrentSongRating() returned correct ranking of ");
					this->m_pLog->appendNumber(testSongRanking);
					this->m_pLog->Log_OutputAppendedText();
					this->m_testResult_Success("GetCurrentSongRating()");
				}
				else
				{
					this->m_pLog->appendText("Error: GetCurrentSongRating() returned incorrect ranking of ");
					this->m_pLog->appendNumber(testSongRanking);
					this->m_pLog->appendText(" should be ");
					this->m_pLog->appendNumber(itRandUserPlaylist->second[0].rating);
					this->m_pLog->Log_OutputAppendedText();
					this->m_testResult_Failure("GetCurrentSongRating()");
				}

			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test GetCurrentSongRating()");
			this->m_testResult_Failure("GetCurrentSongRating()", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing GetCurrentSongRating() 


	this->m_pLog->LogPrintLineBreak(60);


	{// Testing GetUsersSongLibraryAscendingBySongRating() 
		this->m_pLog->Log("Testing GetUsersSongLibraryAscendingBySongRating()");
		this->m_addTestResult("GetUsersSongLibraryAscendingBySongRating()");
		this->m_addTestResult("GetUsersSongLibrary()");

		// Pick random user with a playlist of at least 3 songs
		unsigned int randUserSongifyID = 0;
		std::vector<cSong> vecRandUserPlaylist;
		if (!this->m_getRandTestUserPlaylistWithMinimumNumSongs(randUserSongifyID, vecRandUserPlaylist, 3))
		{
			// Shouldn't happen unless test run we very few songs and-or few users
			// Test playlists are generated with up to 25% of the number of songs in m_vecSomeSongs vector
			this->m_pLog->Log("TESTER ERROR: No users found with large enough playlist (3 songs).");
			this->m_pLog->Log("TESTER ERROR: Run again with more users and/or more songs.");
			assert(!"TESTER ERROR: No users found with large enough playlist (3 songs). Run again wth more users and/or songs.");
		}

		// Find this user in Songify
		cPerson foundPerson;
		if (this->m_pSongify->FindUser(randUserSongifyID, foundPerson))
		{
			this->m_pLog->Log("(CORRECT) FindUser() returned true");
			this->m_testResult_Success("FindUser()");
			// Get reference person
			cPerson* pRandomPerson = this->m_get_pUserByID(randUserSongifyID);
			cPerson theRandomPerson = *pRandomPerson;
			// Returned same person?
			std::vector<std::string> vecDiffences;
			if (!comparePeople(theRandomPerson, foundPerson, vecDiffences))
			{
				// FindUser() returned a different person
				this->m_pLog->Log("Error: FoundUser() returned a different person than expected");
				this->m_pLog->Log(" - expected: " + printablePerson(theRandomPerson));
				this->m_pLog->Log(" - got:      " + printablePerson(foundPerson));
				this->m_testResult_Failure("FindUser()");
				this->m_pLog->Log(" - Without FindUser() working, can't test UpdateSong()");
				this->m_testResult_Failure("GetUsersSongLibraryAscendingBySongRating()", "FindUser() doesn't work");
			}
			else
			{
				// FindUser() returned the correct (matching) person

				// Update the middle song from playlist
				std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(randUserSongifyID);

				// Save the currentRatings
				std::vector< std::pair<unsigned int, unsigned int> > vecSongID_OldRatings;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					vecSongID_OldRatings.push_back(std::pair<unsigned int, unsigned int>(itSong->getUniqueID(), itSong->rating));
				}

				// Update the ratings
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					// (rating, by the USER, from 0 to 5)
					unsigned int newRating = rand() % 6;
					itSong->rating = newRating;
				}

				this->m_pLog->appendText("Updating ");
				this->m_pLog->appendNumber(itRandUserPlaylist->second.size());
				this->m_pLog->appendText(" song ratings in playlist");
				this->m_pLog->Log_OutputAppendedText();

				// Update the ratings to match
				unsigned int numErrors = 0;
				for (std::vector<cSong>::iterator itSong = itRandUserPlaylist->second.begin();
					itSong != itRandUserPlaylist->second.end(); itSong++)
				{
					if (!this->m_pSongify->UpdateRatingOnSong(foundPerson.getSongifyUniqueUserID(), itSong->getUniqueID(), itSong->rating))
					{
						numErrors++;
					}
				}//for ( std::vector<cSong>::iterator...

				// How many worked?
				if (numErrors == 0)
				{
					this->m_pLog->Log("(CORRECT) UpdateRatingOnSong() returned true every time");
					this->m_testResult_Success("UpdateRatingOnSong()");
				}
				else
				{
					this->m_pLog->appendText("Error: UpdateRatingOnSong() returned false ");
					this->m_pLog->appendNumber(numErrors);
					this->m_pLog->appendText(" times (should have returned true)");
					this->m_testResult_Failure("UpdateRatingOnSong()");
				}

				// Get updated playlist and compare to see if song ratings match
				cSong* pSongLibrary = nullptr;
				unsigned int sizeOfSongLibrary = 0;
				if (!this->m_pSongify->GetUsersSongLibraryAscendingBySongRating(foundPerson.getSongifyUniqueUserID(), pSongLibrary, sizeOfSongLibrary))
				{
					this->m_pLog->Log("Error: GetUsersSongLibrary() returned false (should have returned true)");
					this->m_testResult_Failure("GetUsersSongLibraryAscendingBySongRating()");
				}
				else
				{
					this->m_pLog->Log("(CORRECT) GetUsersSongLibrary() returned true");
					this->m_testResult_Success("GetUsersSongLibraryAscendingBySongRating()");
				}

				// Does the playlist match? (song removed)
				std::vector<cSong> testPlayList;
				for (unsigned int index = 0; index != sizeOfSongLibrary; index++)
				{
					testPlayList.push_back(pSongLibrary[index]);
				}

				// Retrieve the reference user's playlist again (after the delete)
				itRandUserPlaylist = this->m_mapUser_vecSongPlaylist.find(foundPerson.getSongifyUniqueUserID());


				// Sort the reference playlist by rating
				// Make a copy...
				std::vector<cSong> vecSongsByRanking;
				for (cSong s : itRandUserPlaylist->second)
				{
					vecSongsByRanking.push_back(s);
				}
				class cSortSongByRanking
				{
				public:
					bool operator()(cSong& A, cSong& B)
					{
						// Highest to lowest...
						return (A.rating > B.rating);
					}
				};
				std::sort(vecSongsByRanking.begin(), vecSongsByRanking.end(), cSortSongByRanking());

				sPlaylistCompareParams compareSettings;
				compareSettings.bIgnoreOrder = false;		// <---- *****
				compareSettings.bIgnorePlayCount = true;
				compareSettings.bIgnoreRatings = false;		// <---- *****
				std::vector<std::string> vecDifferences;
				if (this->m_comparePlaylists(vecSongsByRanking, testPlayList, vecDifferences, compareSettings))
				{
					this->m_pLog->Log("(CORRECT) Playlists are in proper ranking order");
					this->m_testResult_Success("GetUsersSongLibraryAscendingBySongRating()");
				}
				else
				{
					this->m_pLog->Log("Error: Playlist are not the same (after song update)");
					this->m_testResult_Failure("GetUsersSongLibraryAscendingBySongRating()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log("They had the following differences:");
						for (std::string line : vecDiffences)
						{
							this->m_pLog->Log(" - " + line);
						}
						this->m_pLog->Log("Reference playlist: ");
						for (cSong s : vecSongsByRanking)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
						this->m_pLog->Log("Test playlist: ");
						for (cSong s : testPlayList)
						{
							this->m_pLog->Log(" - " + printableSong(s));
						}
					}//if ( this->m_bVerboseLogOutput )
				}//if ( this->m_comparePlaylists(...
			}//if ( ! comparePeople(
		}
		else
		{
			this->m_pLog->Log("Error: FindUser() returned false (should have returned true)");
			this->m_testResult_Failure("FindUser()");
			this->m_pLog->Log(" - Without FindUser() working, can't test UpdateRatingOnSong()");
			this->m_testResult_Failure("GetUsersSongLibraryAscendingBySongRating()", "FindUser() doesn't work");

		}//if ( this->m_pSongify->FindUser(

	}// Testing UpdateRatingOnSong() 


	this->m_pLog->LogPrintLineBreak(60);


	{// Testing GetSongToPlay() and GetCurrentSongNumberOfPlays()
		this->m_pLog->Log("Testing GetSongToPlay() and GetCurrentSongNumberOfPlays()");
		this->m_addTestResult("GetSongToPlay()");
		this->m_addTestResult("GetCurrentSongNumberOfPlays()");

		// Go through 100x the number of users and play a bunch of songs
		unsigned int numberOfSongsToPlay = (unsigned int)this->m_vecSomePeople.size() * 100;
		this->m_pLog->appendText("Playing ");
		this->m_pLog->appendNumber(numberOfSongsToPlay);
		this->m_pLog->appendText(" random songs (from the playlists)");
		this->m_pLog->Log_OutputAppendedText();

		unsigned int songPlayCountActual = 0;
		// Times that GetSongToPlay() returned false on existing user + song
		unsigned int numGetSongToPlayReturnedFalseErrors = 0;
		// Times that GetCurrentSongNumberOfPlays() returned wrong number of songs
		unsigned int numGetCurrentSongNumberOfPlaysReturnedFalseErrors = 0;
		unsigned int numGetCurrentSongNumberOfPlaysCountWrong = 0;
		std::vector<std::string> vecSongPlayCountErrors;
		do
		{
			// Pick a random user
			cPerson* pRandRefUser = this->m_getRandom_pUser();
			std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itRandUserPlaylist
				= this->m_mapUser_vecSongPlaylist.find(pRandRefUser->getSongifyUniqueUserID());

			// Pick a random song
			// (Make sure user has a song on their playlist)
			if (!itRandUserPlaylist->second.empty())
			{
				unsigned int refUserSongIndex = rand() % (unsigned int)itRandUserPlaylist->second.size();
				unsigned int refUserSongID = itRandUserPlaylist->second[refUserSongIndex].getUniqueID();

				// Increment plays
				itRandUserPlaylist->second[refUserSongIndex].numberOfTimesPlayed++;

				songPlayCountActual++;	// So we can eventually exit

				// Now update the song in Songify
				cSong foundSong;
				std::string errorString;
				if (!this->m_pSongify->GetSongToPlay(pRandRefUser->getSongifyUniqueUserID(), refUserSongID, foundSong, errorString))
				{
					numGetSongToPlayReturnedFalseErrors++;
					std::stringstream ssError;
					ssError << "Error: GetSongToPlay() returned false on user "
						<< pRandRefUser->getSongifyUniqueUserID() << " and song "
						<< refUserSongID << std::endl;
					vecSongPlayCountErrors.push_back(ssError.str());
					this->m_testResult_Failure("GetSongToPlay()");
				}

				// See if it updated the song count
				unsigned int songFoundNumberOfPlays = 0;
				if (!this->m_pSongify->GetCurrentSongNumberOfPlays(pRandRefUser->getSongifyUniqueUserID(), refUserSongID, songFoundNumberOfPlays))
				{
					// GetCurrentSongNumberOfPlays() returned false - should be true
					std::stringstream ssError;
					ssError << "Error: GetCurrentSongNumberOfPlays() returned false (should be true) on user "
						<< pRandRefUser->getSongifyUniqueUserID() << " and song "
						<< refUserSongID << std::endl;
					vecSongPlayCountErrors.push_back(ssError.str());
					this->m_testResult_Failure("GetCurrentSongNumberOfPlays()");
					numGetCurrentSongNumberOfPlaysReturnedFalseErrors++;
				}
				// Song play count matches?
				if (songFoundNumberOfPlays != itRandUserPlaylist->second[refUserSongIndex].numberOfTimesPlayed)
				{
					std::stringstream ssError;
					ssError << "Error: GetCurrentSongNumberOfPlays() returned incorrect song play count on user "
						<< pRandRefUser->getSongifyUniqueUserID() << " and song "
						<< refUserSongID << std::endl;
					vecSongPlayCountErrors.push_back(ssError.str());
					this->m_testResult_Failure("GetCurrentSongNumberOfPlays()");
					numGetCurrentSongNumberOfPlaysCountWrong++;
				}

			}//if ( ! itRandUserPlaylist->second.empty() )
		} while (songPlayCountActual < numberOfSongsToPlay);


		// At this point, 100xUsers songs have been played
		this->m_pLog->appendText("Played ");
		this->m_pLog->appendNumber(songPlayCountActual);
		this->m_pLog->appendText(" songs");
		this->m_pLog->Log_OutputAppendedText();

		if (numGetSongToPlayReturnedFalseErrors == 0)
		{
			this->m_pLog->Log("(CORRECT) GetSongToPlay() returned true every time");
			this->m_testResult_Success("GetSongToPlay()");
		}
		else
		{
			this->m_pLog->appendText("Error: GetSongToPlay() returned false (should be true) ");
			this->m_pLog->appendNumber(numGetSongToPlayReturnedFalseErrors);
			this->m_pLog->appendText(" times out of ");
			this->m_pLog->appendNumber(songPlayCountActual);
			this->m_pLog->appendText(" attempts");
			this->m_pLog->Log_OutputAppendedText();
			this->m_testResult_Failure("GetSongToPlay()");
		}

		if (numGetCurrentSongNumberOfPlaysReturnedFalseErrors == 0)
		{
			this->m_pLog->Log("(CORRECT) GetCurrentSongNumberOfPlays() returned true every time");
			this->m_testResult_Success("GetCurrentSongNumberOfPlays()");
		}
		else
		{
			this->m_pLog->appendText("Error: GetCurrentSongNumberOfPlays() returned false (should be true) ");
			this->m_pLog->appendNumber(numGetCurrentSongNumberOfPlaysReturnedFalseErrors);
			this->m_pLog->appendText(" times out of ");
			this->m_pLog->appendNumber(songPlayCountActual);
			this->m_pLog->appendText(" attempts");
			this->m_pLog->Log_OutputAppendedText();
			this->m_testResult_Failure("GetCurrentSongNumberOfPlays()");
		}

		if (numGetCurrentSongNumberOfPlaysCountWrong == 0)
		{
			this->m_pLog->Log("(CORRECT) GetCurrentSongNumberOfPlays() returned correct song play count every time");
			this->m_testResult_Success("GetCurrentSongNumberOfPlays()");
		}
		else
		{
			this->m_pLog->appendText("Error: GetCurrentSongNumberOfPlays() returned incorrect song play count ");
			this->m_pLog->appendNumber(numGetCurrentSongNumberOfPlaysCountWrong);
			this->m_pLog->appendText(" times out of ");
			this->m_pLog->appendNumber(songPlayCountActual);
			this->m_pLog->appendText(" attempts");
			this->m_pLog->Log_OutputAppendedText();
			this->m_testResult_Failure("GetCurrentSongNumberOfPlays()");
		}

		// Error summary (playing 100xUsers songs)
		this->m_pLog->appendText("There were ");
		this->m_pLog->appendNumber(vecSongPlayCountErrors.size());
		this->m_pLog->appendText(" errors updating or checking song play count.");
		this->m_pLog->Log_OutputAppendedText();
		if (this->m_bVerboseLogOutput)
		{
			this->m_pLog->Log("Here are the details:");
			for (std::string line : vecSongPlayCountErrors)
			{
				this->m_pLog->Log(" - " + line);
			}
		}//if ( this->m_bVerboseLogOutput )

	}// Testing GetSongToPlay() and GetCurrentSongNumberOfPlays()


	this->m_pLog->LogPrintLineBreak(60);


	{// Testing GetUsersSongLibraryAscendingByNumberOfPlays()
		this->m_pLog->Log("Testing GetUsersSongLibraryAscendingByNumberOfPlays");
		this->m_addTestResult("GetUsersSongLibraryAscendingByNumberOfPlays()");

		unsigned int numPlayListSongErrors = 0;			// Number of mismatched songs in playlist (including incorrect order)
		unsigned int numPlayListsThatMatch = 0;			// +1 if entire playlist matches
		for (cPerson& refPerson : this->m_vecSomePeople)
		{
			std::vector<cSong> vecRefPersonPlaylist;
			this->m_getTestUserPlaylist(refPerson.getSongifyUniqueUserID(), vecRefPersonPlaylist);

			cSong* pTestUserSongLibrary = NULL;
			unsigned int testUserSongLibrarySize = 0;

			bool bUserFound = this->m_pSongify->GetUsersSongLibraryAscendingByNumberOfPlays(refPerson.getSongifyUniqueUserID(), pTestUserSongLibrary, testUserSongLibrarySize);

			if (!bUserFound)
			{
				// Edge case: is that user's playlist empty? (spec not explicit about what it would return)
				if (vecRefPersonPlaylist.empty())
				{
					// Yes, that user doesn't have a playlist
					this->m_pLog->appendText("(CORRECT) GetUsersSongLibraryAscendingByNumberOfPlays() didn't find test user's [");
					this->m_pLog->appendNumber(refPerson.getSongifyUniqueUserID());
					this->m_pLog->appendText("] playlist BUT that user doesn't have a playlist");
					this->m_pLog->Log_OutputAppendedText();
					this->m_testResult_Success("GetUsersSongLibraryAscendingByNumberOfPlays()");
					numPlayListsThatMatch++;
				}
				else
				{
					// Didn't find test user
					this->m_pLog->appendText("Error: GetUsersSongLibraryAscendingByNumberOfPlays() didn't find test user's [");
					this->m_pLog->appendNumber(refPerson.getSongifyUniqueUserID());
					this->m_pLog->appendText("] playlist");
					this->m_pLog->Log_OutputAppendedText();
					this->m_testResult_Failure("GetUsersSongLibraryAscendingByNumberOfPlays()");

				}//if (vecRefPersonPlaylist.empty())

			}//if ( ! bUserFound )


			if (bUserFound)
			{
				// Add user playlist to vector (in order)
				std::vector<cSong> vecTestUserPlaylist;
				for (unsigned int index = 0; index != testUserSongLibrarySize; index++)
				{
					vecTestUserPlaylist.push_back(pTestUserSongLibrary[index]);
				}

				std::vector<std::string> vecDifferences;
				sPlaylistCompareParams comparisonSettings;
				comparisonSettings.bIgnoreOrder = false;		// <---
				comparisonSettings.bIgnorePlayCount = false;	// <---
				comparisonSettings.bIgnoreRatings = true;
				comparisonSettings.bIgnoreSongID = false;
				if (this->m_comparePlaylists(vecRefPersonPlaylist, vecTestUserPlaylist, vecDifferences, comparisonSettings))
				{
					numPlayListsThatMatch++;
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->appendText("(CORRECT) Playlist for user ");
						this->m_pLog->appendNumber(refPerson.getSongifyUniqueUserID());
						this->m_pLog->appendText(" matches");
						this->m_pLog->Log_OutputAppendedText();
					}
					this->m_testResult_Success("GetUsersSongLibraryAscendingByNumberOfPlays()");
				}
				else
				{
					numPlayListSongErrors += (unsigned int)vecDifferences.size();
					this->m_testResult_Failure("GetUsersSongLibraryAscendingByNumberOfPlays()");
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->appendText("Error: Playlist for user ");
						this->m_pLog->appendNumber(refPerson.getSongifyUniqueUserID());
						this->m_pLog->appendText(" does not match");
						this->m_pLog->Log_OutputAppendedText();

						this->m_pLog->Log("Here are the detailed differences:");
						for (std::string line : vecDifferences)
						{
							this->m_pLog->Log(" - " + line);
						}
					}

					for (std::string line : vecDifferences)
					{
						// 1 error per song that's not correct
						this->m_testResult_Failure("GetUsersSongLibraryAscendingByNumberOfPlays()");
					}
				}//if ( this->m_comparePlaylists(

			}//if ( bUserFound )

		}//for ( cPerson &refPerson

		// Summary of all results
		unsigned int numIncorrectPlaylists = (unsigned int)this->m_vecSomePeople.size() - numPlayListsThatMatch;
		if (this->m_vecSomePeople.size() == numPlayListsThatMatch)
		{
			this->m_pLog->appendText("(CORRECT) Tested ");
			this->m_pLog->appendNumber(this->m_vecSomePeople.size());
			this->m_pLog->appendText(" and every playlist matched (zero errors)");
			this->m_pLog->Log_OutputAppendedText();
		}
		else
		{
			this->m_pLog->appendText("Error: Tested ");
			this->m_pLog->appendNumber(this->m_vecSomePeople.size());
			this->m_pLog->appendText(" and only ");
			this->m_pLog->appendNumber(numPlayListsThatMatch);
			this->m_pLog->appendText(" or ");
			float percentMatched = ((float)numPlayListsThatMatch / (float)this->m_vecSomePeople.size()) * 1000.0f;
			percentMatched = (float)((unsigned int)percentMatched) / 10.0f;
			this->m_pLog->appendNumber(percentMatched);
			this->m_pLog->appendText("% playlists matched");
			this->m_pLog->Log_OutputAppendedText();

			this->m_pLog->appendText("Total song mismatch errors: ");
			this->m_pLog->appendNumber(numPlayListSongErrors);
			this->m_pLog->Log_OutputAppendedText();
		}//if ( vecRefPersonPlaylist.size()...


	}// Testing GetUsersSongLibraryAscendingByNumberOfPlays()


	this->m_pLog->Log("RunPlaylistTests() completed.");

	return true;
}



//// Returns true if identical (based on flags)
//// bIgnoreOrder = false --> ALL same songs but different order
//	struct sPlaylistCompareParams
//	{
//		// false -> lists have to match exactly as passed (order as well)
//		//          (so same songs but wrong order = NOT the same)
//		bool bIgnoreOrder = false;	
//		bool bIgnoreRatings = true;
//		bool bIgnorePlayCount = true;
//	};
bool cSongifyTesterExerciser::m_comparePlaylists(std::vector<cSong> refPlaylist,
	std::vector<cSong> testPlaylist,
	std::vector<std::string>& vecDifferences,
	sPlaylistCompareParams comparisonSettings)
{
	if (refPlaylist.empty())
	{
		vecDifferences.push_back("Reference playlist is empty. Can't compare.");
		// EARLY EXIT
		return false;
	}
	if (testPlaylist.empty())
	{
		vecDifferences.push_back("To be tested playlist is empty. Can't compare.");
		// EARLY EXIT
		return false;
	}

	if (refPlaylist.size() != testPlaylist.size())
	{
		std::stringstream ssError;
		ssError
			<< "Playlists aren't the same size. "
			<< "reference has " << refPlaylist.size() << " entries, "
			<< "test has " << testPlaylist.size();
		vecDifferences.push_back(ssError.str());


		if (comparisonSettings.bIgnoreOrder == false)
		{
			vecDifferences.push_back("Can't compare them in order. Changing test to ignore order.");
			comparisonSettings.bIgnoreOrder = true;
		}
	}


	// Will only compare IN ORDER if the playlist are the same size
	// (See code above where that flag is changed if list sizes aren't equal)
	if (!comparisonSettings.bIgnoreOrder)
	{
		// Sort both lists before compare

		class cSongIDSortLT
		{
		public:
			bool operator()(cSong& A, cSong& B)
			{
				return (A.getUniqueID() < B.getUniqueID());
			}
		};

		std::sort(refPlaylist.begin(), refPlaylist.end(), cSongIDSortLT());
		std::sort(testPlaylist.begin(), testPlaylist.end(), cSongIDSortLT());


		bool bAreEqual = true;
		// cSortFunctorSongsIsEqual(bool bIgnoreRatings, bool bIgnorePlayCount, bool bIgnoreSongID)
		cSortFunctorSongsIsEqual funcCompare(comparisonSettings.bIgnoreRatings,
			comparisonSettings.bIgnorePlayCount,
			comparisonSettings.bIgnoreSongID);
		for (unsigned int index = 0; index != refPlaylist.size(); index++)
		{
			if (!funcCompare.areEqual(refPlaylist[index], testPlaylist[index]))
			{
				std::stringstream ssError;
				ssError << "Song at index [" << index << "] doesn't match:\n"
					<< " - ref:  " << printableSong(refPlaylist[index]) << "\n"
					<< " - test: " << printableSong(testPlaylist[index]);
				vecDifferences.push_back(ssError.str());
				bAreEqual = false;
			}
		}
		if (!bAreEqual)
		{
			vecDifferences.push_back("Playlists are NOT equal.");
		}
	}
	else
	{
		// DON'T care about order

		// NOTE: This might be because playlist sizes aren't the same

		// Make a copy of test (so we can remove them)
		std::vector<cSong> vecTestPlaylistCopy;
		for (cSong s : testPlaylist)
		{
			vecTestPlaylistCopy.push_back(s);
		}

		// Scan through ref looking for match in test
		unsigned int songMatches = 0;
		cSortFunctorSongsIsEqual funcCompare(comparisonSettings.bIgnoreRatings,
			comparisonSettings.bIgnorePlayCount,
			comparisonSettings.bIgnoreSongID);
		std::vector<cSong> vecRefMissingSongs;
		for (cSong refSong : refPlaylist)
		{
			bool bFoundSong = false;
			for (std::vector<cSong>::iterator itTestSong = vecTestPlaylistCopy.begin();
				itTestSong != vecTestPlaylistCopy.end();
				itTestSong++)
			{
				if (funcCompare.areEqual(refSong, *itTestSong))
				{
					// Found a match
					bFoundSong = true;
					songMatches++;
					// Remove song
					vecTestPlaylistCopy.erase(itTestSong);
					break;
				}
			}//for ( std::vector<cSong>::iterator

			if (!bFoundSong)
			{
				vecRefMissingSongs.push_back(refSong);
			}

		}//for ( cSong refSong

		// vecRefMissingSongs list songs in reference that AREN'T found
		// vecTestPlaylistCopy contains extra songs on test playlist

		if (!vecRefMissingSongs.empty())
		{
			std::stringstream ssError;
			ssError << "Missing " << vecDifferences.size() << " songs in test playlist (should be there):";
			vecDifferences.push_back(ssError.str());
			for (cSong s : vecRefMissingSongs)
			{
				std::stringstream ssMissing;
				ssMissing << " - " << printableSong(s);
				vecDifferences.push_back(ssMissing.str());
			}
		}

		if (!vecTestPlaylistCopy.empty())
		{
			std::stringstream ssError;
			ssError << "There are " << vecDifferences.size() << " additional songs in test playlist (shouldn't be there):";
			vecDifferences.push_back(ssError.str());
			for (cSong s : vecTestPlaylistCopy)
			{
				std::stringstream ssMissing;
				ssMissing << " - " << printableSong(s);
				vecDifferences.push_back(ssMissing.str());
			}

		}

	}//if ( ! comparisonSettings.bIgnoreOrder )


	return (vecDifferences.empty());
}

bool cSongifyTesterExerciser::m_getTestUserPlaylist(unsigned int songifyID, std::vector<cSong>& vecSongPlaylist)
{
	vecSongPlaylist.clear();

	std::map<unsigned int /*songifyID*/, std::vector<cSong> >::iterator itPlaylist
		= this->m_mapUser_vecSongPlaylist.find(songifyID);

	if (itPlaylist == this->m_mapUser_vecSongPlaylist.end())
	{
		// That user doesn't have a playlist, apparently
		return false;
	}

	for (cSong s : itPlaylist->second)
	{
		vecSongPlaylist.push_back(s);
	}
	return true;
}


//	std::map<unsigned int /*songifyID*/, std::vector<cSong> > m_mapUser_vecSongInPlaylist;
// The songs are saved as a copy, so this updates them if there's any song details update

void cSongifyTesterExerciser::m_updateTestUserPlaylistDataToMatchReferenceSongsList(void)
{
	for (std::map<unsigned int /*songifyID*/, std::vector<cSong>>::iterator itUser = this->m_mapUser_vecSongPlaylist.begin();
		itUser != this->m_mapUser_vecSongPlaylist.end(); itUser++)
	{
		// Make a copy of just the songIDs, ratings, and numplays
		struct sSongMinPlaylistInfo
		{
			unsigned int songID = 0;
			unsigned int rating = 0;
			unsigned int numberOfPlays = 0;
		};
		std::vector<sSongMinPlaylistInfo> vecSongIDsOnPlaylist;
		for (cSong s : itUser->second)
		{
			sSongMinPlaylistInfo sInfo;
			sInfo.songID = s.getUniqueID();
			sInfo.rating = s.rating;
			sInfo.numberOfPlays = s.numberOfTimesPlayed;
			vecSongIDsOnPlaylist.push_back(sInfo);
		}
		// Clear the actual playlist
		itUser->second.clear();

		// Now add back the songs again, if they are still in the main reference list
		for (sSongMinPlaylistInfo songInfo : vecSongIDsOnPlaylist)
		{
			class cSongIDMatch
			{
			public:
				unsigned int m_songID = 0;
				cSongIDMatch(unsigned int songID)
				{
					this->m_songID = songID;
				}
			public:
				bool operator()(cSong& S)
				{
					return (this->m_songID == S.getUniqueID());
				}
			};
			std::vector<cSong>::iterator itActualSong = std::find_if(this->m_vecSomeSongs.begin(), this->m_vecSomeSongs.end(), cSongIDMatch(songInfo.songID));

			// If it didn't find it, it's likely because that song was deleted from the main-reference song list
			if (itActualSong != this->m_vecSomeSongs.end())
			{
				// It exists, so add it back WITH the user info
				cSong sToAdd = *itActualSong;
				sToAdd.rating = songInfo.rating;
				sToAdd.numberOfTimesPlayed = songInfo.numberOfPlays;
				itUser->second.push_back(sToAdd);
			}
		}//for ( sSongMinPlaylistInfo songInfo...
	}//for ( std::map<..>::iterator

	return;
}


// Returns false if no user has that many songs
bool cSongifyTesterExerciser::m_getRandTestUserPlaylistWithMinimumNumSongs(unsigned int& songifyID, std::vector<cSong>& vecSongPlaylist, unsigned int minNumberOfSongs)
{

	// Get any playlists large enough
	struct sPersonAndPlaylist
	{
		unsigned int songifyID = 0;
		std::vector<cSong> vecSongPlaylist;
	};
	std::vector< sPersonAndPlaylist > vecPeopleAndPlaylists;
	for (cPerson p : this->m_vecSomePeople)
	{
		// Playlist is at large enough
		sPersonAndPlaylist ppl;
		ppl.songifyID = p.getSongifyUniqueUserID();
		this->m_getTestUserPlaylist(p.getSongifyUniqueUserID(), ppl.vecSongPlaylist);
		if (ppl.vecSongPlaylist.size() >= minNumberOfSongs)
		{
			//Playlist is large enough, so save it
			vecPeopleAndPlaylists.push_back(ppl);
		}
	}

	// Did we find any? 
	if (vecPeopleAndPlaylists.empty())
	{
		// No
		return false;
	}

	// Pick a random person
#if _MSVC_LANG < 201703L			//__cplusplus
	// Committe deprecated this after C++14 (because they are infuriating)
	std::random_shuffle(vecPeopleAndPlaylists.begin(), vecPeopleAndPlaylists.end());
#else
	unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(vecPeopleAndPlaylists.begin(), vecPeopleAndPlaylists.end(), std::default_random_engine(seed));
#endif

	// Pick the first one (in the now randomly organized vector)
	songifyID = vecPeopleAndPlaylists[0].songifyID;
	vecSongPlaylist.clear();
	for (cSong s : vecPeopleAndPlaylists[0].vecSongPlaylist)
	{
		vecSongPlaylist.push_back(s);
	}

	return true;
}
