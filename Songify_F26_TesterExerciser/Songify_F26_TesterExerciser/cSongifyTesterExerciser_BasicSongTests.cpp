#include "cSongifyTesterExerciser.h"
#include "formattingHelperFunctions.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

bool cSongifyTesterExerciser::RunBasicSongTests(bool bVerboseLogOutput /*=true*/)
{
	if (!this->m_pLog)
	{
		this->m_pLog = new cLogger(this->m_studentInfo);
		this->m_pLog->bAddTimeToLogLines = true;
	}

	this->m_bVerboseLogOutput = bVerboseLogOutput;

	this->m_pLog->LogPrintLineBreak(60);
	this->m_pLog->Log("RunBasicSongTests() starting.");
	if (!this->m_bVerboseLogOutput)
	{
		this->m_pLog->Log("(Generating limited/non-verbose output)");
	}
	this->m_pLog->LogPrintLineBreak(60);

	{// Add a bunch of songs
		this->m_pLog->Log("Adding a bunch of songs with AddSong()");
		this->m_addTestResult("AddSong()");
		std::string errorString = "";
		unsigned int errorCount = 0;
		for (unsigned int count = 0; count != this->m_vecSomeSongs.size(); count++)
		{
			if (this->m_pSongify->AddSong(this->m_vecSomeSongs[count], errorString))
			{
				if (this->m_bVerboseLogOutput)
				{
					this->m_pLog->Log("(CORRECT) Added song: " + printableSong(this->m_vecSomeSongs[count]));
					this->m_testResult_Success("AddSong()");
				}
			}
			else
			{
				this->m_pLog->Log("Error: Failed to add song: " + printableSong(this->m_vecSomeSongs[count]));
				this->m_pLog->Log(" ..returned this error: " + errorString);
				this->m_testResult_Failure("AddSong()");
				errorCount++;
			}//if ( ! this->m_pSongify->AddSong

		}//for ( unsigned int count = 0...

		this->m_pLog->appendText("Added ");
		this->m_pLog->appendNumber(this->m_vecSomeSongs.size() - errorCount);
		this->m_pLog->appendText("songs.");
		this->m_pLog->Log_OutputAppendedText();
		if (errorCount > 0)
		{
			this->m_pLog->appendText("Error: Failed to add ");
			this->m_pLog->appendNumber(errorCount);
			this->m_pLog->appendText("songs.");
			this->m_pLog->Log_OutputAppendedText();
		}
	}// Add a bunch of songs


	this->m_pLog->LogPrintLineBreak();


	{// Verifying songs are there
		this->m_pLog->Log("Verifying songs are present with FindSong(uniqueID)");
		this->m_addTestResult("FindSong(uniqueID)");
		std::string errorString = "";
		unsigned int errorCount = 0;
		for (unsigned int index = 0; index != this->m_vecSomeSongs.size(); index++)
		{
			cSong foundSong;
			if (this->m_pSongify->FindSong(this->m_vecSomeSongs[index].getUniqueID(), foundSong))
			{
				std::vector<std::string> vecDifferences;
				if (!this->compareSong(this->m_vecSomeSongs[index], foundSong, vecDifferences, true))
				{
					errorCount++;
					this->m_pLog->Log("Error: songs don't match:");
					this->m_pLog->Log(" - Looked for: " + printableSong(this->m_vecSomeSongs[index]));
					this->m_pLog->Log(" - Got this:   " + printableSong(foundSong));
					if (this->m_bVerboseLogOutput)
					{
						this->m_pLog->Log(" - List of differences: ");
						for (std::string line : vecDifferences)
						{
							this->m_pLog->Log("   :" + line);
						}
					}
					this->m_testResult_Failure("FindSong(uniqueID)");
				}//if ( ! this->compareSong...

			}//if ( this->m_pSongify->FindSong...

		}//for ( unsigned int index...

		// 
		unsigned int numSongsFoundOK = (unsigned int)this->m_vecSomeSongs.size() - errorCount;
		if (errorCount == 0)
		{
			this->m_pLog->appendText("(CORRECT): FindSong(uniqueID) found all ");
			this->m_pLog->appendNumber(numSongsFoundOK);
			this->m_pLog->appendText(" songs it was looking for.");
			this->m_pLog->Log_OutputAppendedText();
			this->m_testResult_Success("FindSong(uniqueID)", numSongsFoundOK);
		}
		else
		{
			this->m_pLog->Log("Error: Didn't find some or all songs that were added:");
			this->m_pLog->appendText("Looked for ");
			this->m_pLog->appendNumber(this->m_vecSomeSongs.size());
			this->m_pLog->appendText(" songs and found ");
			this->m_pLog->appendNumber(numSongsFoundOK);
		}

	}// Verifying songs are there


	this->m_pLog->LogPrintLineBreak();


	{// Testing DeleteSong()
		this->m_pLog->Log("Testing DeleteSong()");
		this->m_addTestResult("DeleteSong()");

		cSong songToDelete = *(this->m_getRandom_pSong());
		this->m_pLog->Log("Attempting to delete this song: " + printableSong(songToDelete));

		std::string errorString;
		if (this->m_pSongify->DeleteSong(songToDelete.getUniqueID(), errorString))
		{
			this->m_pLog->Log("(CORRECT) DeleteSong() returned true");
			this->m_testResult_Success("DeleteSong()");
		}
		else
		{
			this->m_pLog->Log("Error: DeleteSong() returned false deleting known song (should be true)");
			this->m_testResult_Failure("DeleteSong()", "DeleteSong() returned false deleting known song (should be true)");
		}
		// Did it actually delete it?
		cSong foundSong;
		if (!this->m_pSongify->FindSong(songToDelete.getUniqueID(), foundSong))
		{
			this->m_pLog->Log("(CORRECT) DeleteSong() deleted the song.");
			this->m_testResult_Success("DeleteSong()");
		}
		else
		{
			// Returned true - what did it return?
			this->m_pLog->Log("Error: FindSong() returned true on song we deleted with DeleteSong()");
			this->m_testResult_Failure("DeleteSong()", "FindSong() returned true on song we deleted with DeleteSong()");

			std::vector<std::string> vecDifferences;
			if (this->compareSong(songToDelete, foundSong, vecDifferences, true))
			{
				this->m_pLog->Log("Error: FindSong() returned passed to DeleteSong()");
				this->m_testResult_Failure("DeleteSong()", "FindSong() returned passed to DeleteSong()");
			}
			else
			{
				this->m_pLog->Log("Error: FindSong() returned a different song than DeleteSong() deleted");
				this->m_pLog->Log(" - deleted this song: " + printableSong(songToDelete));
				this->m_pLog->Log(" - found this song: " + printableSong(foundSong));
				this->m_testResult_Failure("DeleteSong()", "FindSong() returned a different song than DeleteSong() deleted");
			}

		}//if ( ! this->m_pSongify->FindSong

		// Add the song back again
		errorString = "";
		this->m_pLog->Log("Adding the deleted song back again");
		if (this->m_pSongify->AddSong(songToDelete, errorString))
		{
			this->m_pLog->Log("(CORRECT) AddSong() added the deleted song");
			this->m_testResult_Success("AddSong()");
		}
		else
		{
			this->m_pLog->Log("Error: AddSong() returned false adding the deleted song");
			this->m_testResult_Failure("AddSong()");
		}

	}// Testing DeleteSong()


	this->m_pLog->LogPrintLineBreak();

	{// Testing Update Song
		this->m_pLog->Log("Testing UpdateSong()");
		this->m_addTestResult("UpdateSong()");

		cSong* pReferenceSongToUpdate = this->m_getRandom_pSong();
		this->m_pLog->Log("Attempting to update this song: " + printableSong(*pReferenceSongToUpdate));

		cSong newSongInfo = this->m_pTheMusic->getRandomSong();

		// Changing the details to the existing song - but keep the same ID
		pReferenceSongToUpdate->name = newSongInfo.name;
		pReferenceSongToUpdate->artist = newSongInfo.artist;
		pReferenceSongToUpdate->releaseDateYear = newSongInfo.releaseDateYear;
		pReferenceSongToUpdate->releaseDateMonth = newSongInfo.releaseDateMonth;
		pReferenceSongToUpdate->releaseDateDay = newSongInfo.releaseDateDay;

		cSong songToUpdate = *pReferenceSongToUpdate;
		this->m_pLog->Log("To this: " + printableSong(newSongInfo));


		this->m_pLog->Log("Attempting to update this song: " + printableSong(songToUpdate));

		cSong foundSong;
		if (this->m_pSongify->FindSong(songToUpdate.getUniqueID(), foundSong))
		{
			this->m_pLog->Log("FindSong() found the song");

			this->m_pLog->Log("Song now updated to: " + printableSong(songToUpdate));

			std::string errorString;
			if (this->m_pSongify->UpdateSong(songToUpdate, errorString))
			{
				this->m_pLog->Log("(CORRECT) UpdateSong() returned true");
				this->m_testResult_Success("UpdateSong()");

				// Find that song to see if it updated
				cSong songUpdated;
				if (this->m_pSongify->FindSong(songToUpdate.getUniqueID(), songUpdated))
				{
					this->m_pLog->Log("(CORRECT) FindSong() returned true looking for updated song");
					this->m_testResult_Success("UpdateSong()");
					// Is it updated?
					std::vector<std::string> vecDifferences;
					if (this->compareSong(songToUpdate, songUpdated, vecDifferences, false))
					{
						this->m_pLog->Log("(CORRECT) FindSong() returned updated song");
						this->m_testResult_Success("UpdateSong()");
					}
					else
					{
						this->m_pLog->Log("Error: Updated song details are wrong:");
						this->m_pLog->Log(" - Updated song info: " + printableSong(songUpdated));
						if (this->m_bVerboseLogOutput)
						{
							this->m_pLog->Log(" - Here is a detailed list:");
							for (std::string line : vecDifferences)
							{
								this->m_pLog->Log("  -- " + line);
								this->m_testResult_Failure("UpdateSong()");
							}
						}
					}//if ( this->compareSong(
				}
				else
				{
					this->m_pLog->Log("Error: FindSong() returned false searching for updated song (should be true)");
					this->m_testResult_Failure("UpdateSong()");

					// Did it update anyway (did it return the updated song?)
					std::vector<std::string> vecDifferences;
					if (this->compareSong(songToUpdate, songUpdated, vecDifferences, false))
					{
						this->m_pLog->Log("(CORRECT) FindSong() returned updated song");
						this->m_testResult_Success("UpdateSong()");
					}
					else
					{
						this->m_pLog->Log("Error: Updated song details are wrong:");
						this->m_pLog->Log(" - Updated song info: " + printableSong(songUpdated));
						if (this->m_bVerboseLogOutput)
						{
							this->m_pLog->Log(" - Here is a detailed list:");
							for (std::string line : vecDifferences)
							{
								this->m_pLog->Log("  -- " + line);
								this->m_testResult_Failure("UpdateSong()");
							}
						}
					}//if ( this->compareSong(

				}// if ( this->m_pSongify->FindSong(
			}
			else
			{
				this->m_pLog->Log("Error: UpdateSong() returned false (should return true)");
				this->m_testResult_Failure("UpdateSong()");

			}//if ( this->m_pSongify->UpdateSong(		
		}
		else
		{
			this->m_pLog->Log("Error: FindSong() could not find song to update");
			this->m_pLog->Log("Error: Can't test UpdateSong() without existing song");
			this->m_testResult_Failure("FindSong()");
			this->m_testResult_Failure("UpdateSong()", "FindSong() could not find song to update");
		}//if ( this->m_pSongify->FindSong

	}// Testing Update Song


	this->m_pLog->Log("RunBasicSongTests() completed.");

	return true;
}





// Doesn't compare the rating or number-of-times-played
bool cSongifyTesterExerciser::compareSong(cSong& A, cSong& B, std::vector<std::string>& vecDifferences, bool bIncludeSongID)
{
	vecDifferences.clear();

	if (A.name != B.name)
	{
		vecDifferences.push_back("Song names don't match");
	}
	if (A.artist != B.artist)
	{
		vecDifferences.push_back("Artists don't match");
	}
	if (A.releaseDateDay != B.releaseDateDay)
	{
		vecDifferences.push_back("ReleaseDateDays don't match");
	}
	if (A.releaseDateMonth != B.releaseDateMonth)
	{
		vecDifferences.push_back("ReleaseDateMonths don't match");
	}
	if (A.releaseDateYear != B.releaseDateYear)
	{
		vecDifferences.push_back("ReleaseDateYears don't match");
	}
	if (bIncludeSongID)
	{
		if (A.getUniqueID() != B.getUniqueID())
		{
			vecDifferences.push_back("SongIDs don't match");
		}
	}//if (bIncludeSongID)

	return (vecDifferences.empty());
}

// Can compare the ratings, etc. 
bool cSongifyTesterExerciser::compareSongInPlaylist(
	cSong& A, cSong& B, std::vector<std::string>& vecDifferences,
	bool bIgnoreRatings, bool bIgnorePlayCount)
{
	vecDifferences.clear();

	if (A.name != B.name)
	{
		vecDifferences.push_back("Song names don't match");
	}
	if (A.artist != B.artist)
	{
		vecDifferences.push_back("Artists don't match");
	}
	if (A.releaseDateDay != B.releaseDateDay)
	{
		vecDifferences.push_back("ReleaseDateDays don't match");
	}
	if (A.releaseDateMonth != B.releaseDateMonth)
	{
		vecDifferences.push_back("ReleaseDateMonths don't match");
	}
	if (A.releaseDateYear != B.releaseDateYear)
	{
		vecDifferences.push_back("ReleaseDateYears don't match");
	}
	if (A.getUniqueID() != B.getUniqueID())
	{
		vecDifferences.push_back("SongIDs don't match");
	}

	if (!bIgnoreRatings)
	{
		if (A.rating != B.rating)
		{
			vecDifferences.push_back("User ratings don't match");
		}
	}
	if (!bIgnorePlayCount)
	{
		if (A.numberOfTimesPlayed == B.numberOfTimesPlayed)
		{
			vecDifferences.push_back("Number of times played doesn't match");
		}
	}


	return (vecDifferences.empty());


}
