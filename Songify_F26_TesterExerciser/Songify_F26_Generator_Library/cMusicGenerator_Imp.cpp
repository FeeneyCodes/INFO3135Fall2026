#include "cMusicGenerator_Imp.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <float.h>
#include <process.h>	// for the threading
#define WIN32_LEAN_AND_MEAN
#include <windows.h>	// for the threading, specifically WaitForMultipleObjects()

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

cMG_Imp::cMG_Imp()
{
	this->p_theRandomizer = new cMersenneTwister((unsigned long)time(NULL));
}

cMG_Imp::~cMG_Imp()
{
	delete this->p_theRandomizer;


	// Remove any made up songs
	// (the other pointers point to the arrays and are deleted later)
	for (const std::pair<unsigned int, sUniqueSongInfo> &itSong : this->mapUniqueSongIDs_to_SongInfo)
	{
		if (itSong.second.pMadeUpSong != NULL)
		{
			delete itSong.second.pMadeUpSong;
		}
	}


	// Clear indexing maps
	this->mapUniqueSongIDs_to_SongInfo.clear();

	this->map_SongNamePlusPerformer_to_UniqueID.clear();
	this->map_SongName_to_UniqueID.clear();
	this->map_Performer_to_UniqueID.clear();

	this->map_UpperCase_SongNamePlusPerformer_to_UniqueID.clear();
	this->map_UpperCase_SongName_to_UniqueID.clear();
	this->map_UpperCase_Performer_to_UniqueID.clear();

	// Delete the song data arrays
	delete[] this->pAudiioInfoArray;
	delete[] this->pHotStuffInfoArray;

}

// This will tokenize the line at commas (",") but won't treak commas inside quotes
//	as delimiters. Unfortunately, getline() can't differentiate these...
// Returns the number of tokens found
unsigned int tokenizeLineWithQuotes(const std::string& theString, std::vector<std::string> &vecTokens)
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
			bInsideAQuote = ! bInsideAQuote;
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


void cMG_Imp::setProgressCallbackObjectInstance(cCallbackProgress* pCallbackProgressObject)
{
	this->pCallbackObject = pCallbackProgressObject;
	return;
}

void cMG_Imp::logProgressUpdate(std::string updateText)
{
	if (this->pCallbackObject)
	{
		this->pCallbackObject->progressData(updateText);
	}
	return;
}

struct sAudiioThreadParams
{
	cMG_Imp* pMG_Imp = NULL;
	std::vector<std::string>* pVecAudiioFileByLines = NULL;
	unsigned int startIndex = 0;
	unsigned int endIndex = 0;
};

//void processLine(cMG_Imp* pMG_Imp, std::vector<std::string> &vecAudiioFileByLines, unsigned int startIndex, unsigned int endIndex)
DWORD WINAPI processAudiioLine(void* pAudiioThreadInfo)
{
	sAudiioThreadParams* pATP = (sAudiioThreadParams*)pAudiioThreadInfo;

	std::vector<std::string>& vecAudiioFileByLines = *(pATP->pVecAudiioFileByLines);

	for (unsigned int lineIndex = pATP->startIndex; lineIndex != pATP->endIndex; lineIndex++)
	{
		std::string& theLine = vecAudiioFileByLines[lineIndex];

		std::vector<std::string> vecTokens;
		tokenizeLineWithQuotes(theLine, vecTokens);

		// Get pointer to matching entry
		cMG_Imp::sAudiioCSVFileInfo* pAudioEntryLine = &(pATP->pMG_Imp->pAudiioInfoArray[lineIndex]);

		for (unsigned int tokenIndex = 0; tokenIndex != (unsigned int)vecTokens.size(); tokenIndex++)
		{
			switch (tokenIndex)
			{
			case 0:		// std::string songid
				pAudioEntryLine->songid = vecTokens[tokenIndex];
				break;
			case 1:		// std::string performer
				pAudioEntryLine->performer = vecTokens[tokenIndex];
				break;
			case 2:		// std::string song
				pAudioEntryLine->song = vecTokens[tokenIndex];
				break;
			case 3:		// std::string spotify_genre
				pAudioEntryLine->spotify_genre = vecTokens[tokenIndex];
				break;
			case 4:		// std::string spotify_track_id
				pAudioEntryLine->spotify_track_id = vecTokens[tokenIndex];
				break;
			case 5:		// std::string spotify_track_preview_url
				pAudioEntryLine->spotify_track_preview_url = vecTokens[tokenIndex];
				break;
			case 6:		// std::string spotify_track_album
				pAudioEntryLine->spotify_track_album = vecTokens[tokenIndex];
				break;
			case 7:		// bool spotify_track_explicit
				pAudioEntryLine->spotify_track_explicit = (vecTokens[tokenIndex] == "TRUE" ? true : false);
				break;
			case 8:		// unsigned int spotify_track_duration_ms
				pAudioEntryLine->spotify_track_duration_ms = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 9:		// unsigned int spotify_track_popularity
				pAudioEntryLine->spotify_track_popularity = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 10:	// float danceability
				pAudioEntryLine->danceability = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 11:	// float energy
				pAudioEntryLine->energy = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 12:	// unsigned int key
				pAudioEntryLine->key = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 13:	// float loudness
				pAudioEntryLine->loudness = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 14:	// bool mode
				pAudioEntryLine->mode = (vecTokens[tokenIndex] == "TRUE" ? true : false);
				break;
			case 15:	// float speechiness
				pAudioEntryLine->speechiness = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 16:	// double acousticness
				pAudioEntryLine->acousticness = atof(vecTokens[tokenIndex].c_str());
				break;
			case 17:	// double instrumentalness
				pAudioEntryLine->instrumentalness = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 18:	// float liveness
				pAudioEntryLine->liveness = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 19:	// float valence
				pAudioEntryLine->valence = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 20:	// float tempo
				pAudioEntryLine->tempo = (float)atof(vecTokens[tokenIndex].c_str());
				break;
			case 21:	// unsigned int time_signature
				pAudioEntryLine->time_signature = atoi(vecTokens[tokenIndex].c_str());
				break;
			default:	// std::vector<std::string> vecExtraData
				pAudioEntryLine->vecExtraData.push_back(vecTokens[tokenIndex]);
			}//switch (tokenIndex)

		}//for (tokenIndex...

		// Tokenize the genres
		std::stringstream ssGenresCleaned;
		for (unsigned int charIndex = 0; charIndex != pAudioEntryLine->spotify_genre.size(); charIndex++)
		{
			char curChar = pAudioEntryLine->spotify_genre[charIndex];
			if (curChar == '[' || curChar == ']' || curChar == '\'')
			{
				continue;
			}
			ssGenresCleaned << curChar;
		}
		std::string singleGenre;
		while (std::getline(ssGenresCleaned, singleGenre, ','))
		{
			pAudioEntryLine->vec_spotify_genres.push_back(singleGenre);
		}

	} // for (lineIndex...

	return 0;
}

struct sHotStuffThreadParams
{
	cMG_Imp* pMG_Imp = NULL;
	std::vector<std::string>* pVecHotStuffFileByLines = NULL;
	unsigned int startIndex = 0;
	unsigned int endIndex = 0;
};

//void processLine(cMG_Imp* pMG_Imp, std::vector<std::string> &vecAudiioFileByLines, unsigned int startIndex, unsigned int endIndex)
DWORD WINAPI processHotStuffLine(void* pHotStuffThread)
{
	sHotStuffThreadParams* pHSP = (sHotStuffThreadParams*)pHotStuffThread;

	std::vector<std::string>& vecAudiioFileByLines = *(pHSP->pVecHotStuffFileByLines);

	for (unsigned int lineIndex = pHSP->startIndex; lineIndex != pHSP->endIndex; lineIndex++)
	{
		std::string& theLine = vecAudiioFileByLines[lineIndex];

		std::vector<std::string> vecTokens;
		tokenizeLineWithQuotes(theLine, vecTokens);

		// Get pointer to matching entry
		cMG_Imp::sHotStuffCSVInfo* pHotStuffEntryLine = &(pHSP->pMG_Imp->pHotStuffInfoArray[lineIndex]);

//		sHotStuffCSVInfo* pHotStuffEntryLine = new sHotStuffCSVInfo();

		// url,weekid,week_position,song,performer,songid,instance,previous_week_position,peak_position,weeks_on_chart
		for (unsigned int tokenIndex = 0; tokenIndex != (unsigned int)vecTokens.size(); tokenIndex++)
		{
			switch (tokenIndex)
			{
			case 0:		// std::string url
				pHotStuffEntryLine->url = vecTokens[tokenIndex];
				break;
			case 1:		// std::string weekid
				pHotStuffEntryLine->weekid = vecTokens[tokenIndex];
				{
					//sDate weekid_Parsed;
					// file format is "1958-08-02"
					pHotStuffEntryLine->weekid_Parsed.year = atoi(pHotStuffEntryLine->weekid.substr(0, 4).c_str());
					pHotStuffEntryLine->weekid_Parsed.month = atoi(pHotStuffEntryLine->weekid.substr(5, 2).c_str());
					pHotStuffEntryLine->weekid_Parsed.day = atoi(pHotStuffEntryLine->weekid.substr(8, 2).c_str());
				}
				break;
			case 2:		// unsigned int week_position
				pHotStuffEntryLine->previous_week_position = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 3:		// std::string song
				pHotStuffEntryLine->song = vecTokens[tokenIndex];
				break;
			case 4:		// std::string performer
				pHotStuffEntryLine->performer = vecTokens[tokenIndex];
				break;
			case 5:		// std::string songid
				pHotStuffEntryLine->songid = vecTokens[tokenIndex];
				break;
			case 6:		// unsigned int instance
				pHotStuffEntryLine->instance = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 7:		// unsigned int previous_week_position
				pHotStuffEntryLine->previous_week_position = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 8:		// unsigned int peak_position
				pHotStuffEntryLine->peak_position = atoi(vecTokens[tokenIndex].c_str());
				break;
			case 9:		// unsigned int weeks_on_chart
				pHotStuffEntryLine->weeks_on_chart = atoi(vecTokens[tokenIndex].c_str());
				break;
			default:	// std::vector<std::string> vecExtraData
				pHotStuffEntryLine->vecExtraData.push_back(vecTokens[tokenIndex]);
			}//switch (tokenIndex)

		}//for (tokenIndex...

	}//for (..lineIndex...


	return 0;
}


bool cMG_Imp::LoadMusicInformation(std::string musicFileName, std::string& errorString)
{
//                        _  _  _                           
//       __ _  _   _   __| |(_)(_)  ___     ___  ___ __   __
//      / _` || | | | / _` || || | / _ \   / __|/ __|\ \ / /
//     | (_| || |_| || (_| || || || (_) |_| (__ \__ \ \ V / 
//      \__,_| \__,_| \__,_||_||_| \___/(_)\___||___/  \_/  
//                                                          
	// If blank, assume it's "audiio.csv"
	if (musicFileName == "")
	{
		musicFileName = "audiio.csv";
	}

	// Open the file
	std::ifstream musicFile(musicFileName.c_str());
	if (!musicFile.is_open())
	{
		errorString = "Can't open file";
		return false;
	}

//
	this->logProgressUpdate("Loading audiio.csv file...\n");

	musicFile.seekg(0, std::ios::end);
	std::ios::pos_type fileSize = musicFile.tellg();
	// Return to start
	musicFile.seekg(0, std::ios::beg);

	char* pRawMusicFileData = new char[static_cast<unsigned int>(fileSize)];

	musicFile.read(pRawMusicFileData, fileSize);
	musicFile.close();

	this->logProgressUpdate("File loaded.\n");

	// Used for progress update
	unsigned int tenPercentOf_audiio_FileSize = (unsigned int)fileSize / 10;


	// Create stream for getline()
	std::stringstream ssFileData(std::string(pRawMusicFileData, (std::size_t)fileSize));

	unsigned int lineCount = 0;
	unsigned int bytesProcessed = 0;
	// We can't know exaclty 10% so this is approximately that
	unsigned int tenPercentByteCount = 0;

	this->logProgressUpdate("Parsing file...\n");

	std::vector<std::string> vecAudiioFileByLines;

	std::string theLine;
	while (std::getline(ssFileData, theLine))
	{
		lineCount++;

//		bytesProcessed += (unsigned int)theLine.size();
//		tenPercentByteCount += (unsigned int)theLine.size();
//
//		// Approximately next 10% processed?
//		if (tenPercentByteCount >= tenPercentOf_audiio_FileSize)
//		{
//			unsigned int percentDone = (unsigned int)(((float)bytesProcessed / (float)fileSize) * 100.0f);
//			std::stringstream ssPercentDone;
//			ssPercentDone << percentDone << "% done\n";
//			this->logProgressUpdate(ssPercentDone.str());
//			// reset counter
//			tenPercentByteCount = 0;
//		}

		// 1st line has header info, so skip
		if (lineCount == 1)
		{
			continue;
		}
		if (theLine.empty())
		{
			continue;
		}
		// HACK:
		// Sometimes the last line is garbage.
		// See if this line is WAY too long
		// I'm assuming this is something to do with the parsting the stringstream object
		if (theLine.length() > 1000)
		{
			continue;
		}

		vecAudiioFileByLines.push_back(theLine);

	}//while (std::getline(ssFileData, theLine))

	// Allocate all entries
	this->numberOfAudiioInfoElements = (unsigned int)vecAudiioFileByLines.size();
	this->pAudiioInfoArray = new sAudiioCSVFileInfo[this->numberOfAudiioInfoElements];

	// Parse each line
	if (this->bUseMultiThreadedParse)
	{
		// You can mess around with this...
		// On a XEON 8280, 4 seems like the "sweet spot", but more threads actually slow 
		//	things down with the audiio file..
		// So a different number of threads might be better, maybe.
		const unsigned int NUM_THREADS = 4;

		HANDLE hThreads[NUM_THREADS] = { 0 };
		unsigned int threadIDs[NUM_THREADS] = { 0 };
		sAudiioThreadParams AudiioThreadParams[NUM_THREADS];

		unsigned int lineStartIndex = 0;
		unsigned int lineStep = this->numberOfAudiioInfoElements / NUM_THREADS;

		for (unsigned int threadIndex = 0; threadIndex != NUM_THREADS; threadIndex++)
		{
			AudiioThreadParams[threadIndex].pMG_Imp = this;
			AudiioThreadParams[threadIndex].pVecAudiioFileByLines = &vecAudiioFileByLines;
			AudiioThreadParams[threadIndex].startIndex = lineStartIndex;
			AudiioThreadParams[threadIndex].endIndex = lineStartIndex + lineStep;
			// To far (past the end of the audiio data array?
			if (AudiioThreadParams[threadIndex].endIndex > (unsigned int)this->numberOfAudiioInfoElements)
			{
				// Yes, so clamp this threads last line to actual end of array
				AudiioThreadParams[threadIndex].endIndex = (unsigned int)this->numberOfAudiioInfoElements;
			}

			hThreads[threadIndex] = CreateThread(NULL, 0, processAudiioLine, 
			                                     (void*)&(AudiioThreadParams[threadIndex]), 0, 0);

			lineStartIndex += lineStep;
		}

		const DWORD MAX_WAIT_TIME_MS = 60 * 1000;	// 60 seconds per thread?
		WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, MAX_WAIT_TIME_MS);

	}
	else
	{
		// Use just this current thread
		sAudiioThreadParams* pAudiioLineData = new sAudiioThreadParams();
		pAudiioLineData->pMG_Imp = this;
		pAudiioLineData->pVecAudiioFileByLines = &vecAudiioFileByLines;
		pAudiioLineData->startIndex = 0;
		pAudiioLineData->endIndex = (unsigned int)vecAudiioFileByLines.size();

		processAudiioLine( (void*)pAudiioLineData );
	}

	// Calculate unique IDs
	// (Non-threaded because of the STL map access)
	for (unsigned int index = 0; index != this->numberOfAudiioInfoElements; index ++)
	{
		// Get pointer to matching entry
		cMG_Imp::sAudiioCSVFileInfo* pAudiioEntryLine = &(this->pAudiioInfoArray[index]);

		bool bSongID_OK = false;
		do
		{

			pAudiioEntryLine->uniqueSongID = (this->p_theRandomizer->getNextRand() % ULONG_MAX) + 1;
			// Really unique?
			std::map<unsigned int, sUniqueSongInfo>::iterator itSongID = this->mapUniqueSongIDs_to_SongInfo.find(pAudiioEntryLine->uniqueSongID);
			if (itSongID == this->mapUniqueSongIDs_to_SongInfo.end())
			{
				// Yup, it's unique, so add it
				sUniqueSongInfo songInfoEntry;
				songInfoEntry.pAudiioSong = pAudiioEntryLine;
				this->mapUniqueSongIDs_to_SongInfo[pAudiioEntryLine->uniqueSongID] = songInfoEntry;
				bSongID_OK = true;
			}
		} while (!bSongID_OK);

		// Add to indexes
		
		// The "songid" is songName + performer
		this->map_SongNamePlusPerformer_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(pAudiioEntryLine->songid, pAudiioEntryLine->uniqueSongID));
		this->map_Performer_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(pAudiioEntryLine->performer, pAudiioEntryLine->uniqueSongID));
		this->map_SongName_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(pAudiioEntryLine->song, pAudiioEntryLine->uniqueSongID));

		// Now upper case for non-case sensitive searches
		std::string songNameUpper = pAudiioEntryLine->song;
		std::string artistUpper = pAudiioEntryLine->performer;
		for (char& curChar : songNameUpper)
		{
			curChar = toupper(curChar);
		}
		for (char& curChar : artistUpper)
		{
			curChar = toupper(curChar);
		}
		this->map_UpperCase_SongNamePlusPerformer_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(std::string(songNameUpper + artistUpper), pAudiioEntryLine->uniqueSongID));
		this->map_UpperCase_Performer_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(artistUpper, pAudiioEntryLine->uniqueSongID));
		this->map_UpperCase_SongName_to_UniqueID.insert(
			std::pair<std::string, unsigned int>(songNameUpper, pAudiioEntryLine->uniqueSongID));

	}//for (unsigned int index


	delete[] pRawMusicFileData;

	this->logProgressUpdate("Parsing complete.\n");



//      _             _              _            __   __     ____                      
//     | |__    ___  | |_       ___ | |_  _   _  / _| / _|   |___ \     ___  ___ __   __
//     | '_ \  / _ \ | __|     / __|| __|| | | || |_ | |_      __) |   / __|/ __|\ \ / /
//     | | | || (_) || |_      \__ \| |_ | |_| ||  _||  _|    / __/  _| (__ \__ \ \ V / 
//     |_| |_| \___/  \__|_____|___/ \__| \__,_||_|  |_|_____|_____|(_)\___||___/  \_/  
//                       |_____|                       |_____|                          
	std::string hotstuffFileName = "hot_stuff_2.csv";

	// Load the "hot stuff" file, too?
	if (this->bLoadHotStuffFile)
	{
		// Get the path to the "hot stuff" file, assuming it's in the same folder
		// 
		// Are there any slashes (i.e. a path)?
		char slashCharacter = '\0';	// assume no slash character
		if (musicFileName.find("\\") != std::string::npos)
		{
			slashCharacter = '\\';
		}
		else if (musicFileName.find("/") != std::string::npos)
		{
			slashCharacter = '/';
		}

		// Any slashes found?
		if (slashCharacter != '\0')
		{
			// Yup, so find the last one
			// e.g. if passed file was, say: "Data/billboard/kcmillersean-billboard-hot-100-1958-2017/data/audiio.csv"
			//
			std::size_t lastSlashIndex = musicFileName.find_last_of(slashCharacter);

			// Get path and append "hot stuff" file name
			hotstuffFileName = slashCharacter + hotstuffFileName;
			hotstuffFileName = musicFileName.substr(0, lastSlashIndex) + hotstuffFileName;

		}//if (slashCharacter != '\0')


		// Open the file
		std::ifstream hotStuffFile(hotstuffFileName.c_str());
		if (!hotStuffFile.is_open())
		{
			errorString = "Can't open file";
			return false;
		}

		this->logProgressUpdate("Loading hot_stuff_2.csv file...\n");

		//
		hotStuffFile.seekg(0, std::ios::end);
		std::ios::pos_type fileSize = hotStuffFile.tellg();
		// Return to start
		hotStuffFile.seekg(0, std::ios::beg);

		char* pHotStuffRawData = new char[static_cast<unsigned int>(fileSize)];

		hotStuffFile.read(pHotStuffRawData, fileSize);
		hotStuffFile.close();

		this->logProgressUpdate("File loaded.\n");

		// Create stream for getline()
		std::stringstream ssFileData(std::string(pHotStuffRawData, fileSize));

		// Used for progress update
		unsigned int tenPercentOf_hot_stuff_2_FileSize = (unsigned int)fileSize / 10;


		unsigned int bytesProcessed = 0;
		// We can't know exaclty 10% so this is approximately that
		unsigned int tenPercentByteCount = 0;

		this->logProgressUpdate("Splitting lines...");

		unsigned int lineCount = 0;
		std::string theLine;
		std::vector<std::string> vecHotStuffLines;
		while (std::getline(ssFileData, theLine))
		{
			lineCount++;

			bytesProcessed += (unsigned int)theLine.size();
			tenPercentByteCount += (unsigned int)theLine.size();

//			// Approximately next 10% processed?
//			if (tenPercentByteCount >= tenPercentOf_hot_stuff_2_FileSize)
//			{
//				unsigned int percentDone = (unsigned int)(((float)bytesProcessed / (float)fileSize) * 100.0f);
//				std::stringstream ssPercentDone;
//				ssPercentDone << percentDone << "% done\n";
//				this->logProgressUpdate(ssPercentDone.str());
//				// reset counter
//				tenPercentByteCount = 0;
//			}

			// 1st line has header info, so skip
			if (lineCount == 1)
			{
				continue;
			}
			if (theLine.empty())
			{
				continue;
			}


			vecHotStuffLines.push_back(theLine);


		}//while (std::getline(ssFileData, theLine))
		this->logProgressUpdate("done.\n");


		// Allocate all entries
		this->numberOfHotStuffElements = (unsigned int)vecHotStuffLines.size();
		this->pHotStuffInfoArray = new sHotStuffCSVInfo[this->numberOfHotStuffElements];

		// Parse each line
		if (this->bUseMultiThreadedParse)
		{
			// You can mess around with this...
			// Same note as above - will too many threads, it gets slower (cache fighting, I'm guessing)
			// On a XEON 8280, 8 seems like the "sweet spot"
			const unsigned int NUM_THREADS = 8;

			std::stringstream ssLogText;
			ssLogText << "Parsing lines with " << NUM_THREADS << " threads...";
			this->logProgressUpdate(ssLogText.str());

			HANDLE hThreads[NUM_THREADS] = { 0 };
			unsigned int threadIDs[NUM_THREADS] = { 0 };
			sHotStuffThreadParams HotStuffThreadParams[NUM_THREADS];

			unsigned int lineStartIndex = 0;
			unsigned int lineStep = this->numberOfHotStuffElements / NUM_THREADS;

			for (unsigned int threadIndex = 0; threadIndex != NUM_THREADS; threadIndex++)
			{
				HotStuffThreadParams[threadIndex].pMG_Imp = this;
				HotStuffThreadParams[threadIndex].pVecHotStuffFileByLines = &vecHotStuffLines;
				HotStuffThreadParams[threadIndex].startIndex = lineStartIndex;
				HotStuffThreadParams[threadIndex].endIndex = lineStartIndex + lineStep;

				// To far (past the end of the audiio data array?
				if (HotStuffThreadParams[threadIndex].endIndex > (unsigned int)this->numberOfHotStuffElements)
				{
					// Yes, so clamp this threads last line to actual end of array
					HotStuffThreadParams[threadIndex].endIndex = (unsigned int)this->numberOfHotStuffElements;
				}

				hThreads[threadIndex] = CreateThread(NULL, 0, processHotStuffLine,
					(void*)&(HotStuffThreadParams[threadIndex]), 0, 0);

				lineStartIndex += lineStep;
			}

			const DWORD MAX_WAIT_TIME_MS = 60 * 1000;	// 60 seconds per thread?
			WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, MAX_WAIT_TIME_MS);

			this->logProgressUpdate("done.\n");
		}
		else
		{
			// Use just this current thread
			sHotStuffThreadParams* pHotStuffLineData = new sHotStuffThreadParams();
			pHotStuffLineData->pMG_Imp = this;
			pHotStuffLineData->pVecHotStuffFileByLines = &vecHotStuffLines;
			pHotStuffLineData->startIndex = 0;
			pHotStuffLineData->endIndex = (unsigned int)vecHotStuffLines.size();

			this->logProgressUpdate("Parsing lines...");

			processHotStuffLine((void*)pHotStuffLineData);

			this->logProgressUpdate("done.\n");

		}

		this->logProgressUpdate("Generating unique IDs...");
		// Calculate unique IDs
		// (Non-threaded because of the STL map access)
		for (unsigned int index = 0; index != this->numberOfHotStuffElements ; index++)
		{
			// Get pointer to matching entry

			sHotStuffCSVInfo* pHotStuffEntryLine = &(this->pHotStuffInfoArray[index]);

			bool bSongID_OK = false;
			do
			{

				pHotStuffEntryLine->uniqueSongID = (this->p_theRandomizer->getNextRand() % ULONG_MAX) + 1;
				// Really unique?
				std::map<unsigned int, sUniqueSongInfo>::iterator itSongID = this->mapUniqueSongIDs_to_SongInfo.find(pHotStuffEntryLine->uniqueSongID);

				if (itSongID == this->mapUniqueSongIDs_to_SongInfo.end())
				{
					// It is u
					sUniqueSongInfo songInfoEntry;
					songInfoEntry.pHotStuffSong = pHotStuffEntryLine;
					this->mapUniqueSongIDs_to_SongInfo[pHotStuffEntryLine->uniqueSongID] = songInfoEntry;
					bSongID_OK = true;
				}
				//else
				//{
				//	// Found a duplicate
				//	bSongID_OK = false;
				//}
			} while (!bSongID_OK);

			// Add to indexes
			
			// For "hot stuff", the "songid" is songName + performer
			this->map_SongNamePlusPerformer_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(pHotStuffEntryLine->songid, pHotStuffEntryLine->uniqueSongID));
			this->map_Performer_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(pHotStuffEntryLine->performer, pHotStuffEntryLine->uniqueSongID));
			this->map_SongName_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(pHotStuffEntryLine->song, pHotStuffEntryLine->uniqueSongID));

			// Now upper case for non-case sensitive searches
			std::string songNameUpper = pHotStuffEntryLine->song;
			std::string artistUpper = pHotStuffEntryLine->performer;
			for (char& curChar : songNameUpper)
			{
				curChar = toupper(curChar);
			}
			for (char& curChar : artistUpper)
			{
				curChar = toupper(curChar);
			}
			this->map_UpperCase_SongNamePlusPerformer_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(std::string(songNameUpper + artistUpper), pHotStuffEntryLine->uniqueSongID));
			this->map_UpperCase_Performer_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(artistUpper, pHotStuffEntryLine->uniqueSongID));
			this->map_UpperCase_SongName_to_UniqueID.insert(
				std::pair<std::string, unsigned int>(songNameUpper, pHotStuffEntryLine->uniqueSongID));

		}//for (unsigned int index = 0
		this->logProgressUpdate("done.\n");


		this->logProgressUpdate("Parsing complete.\n");

	}// if (this->bLoadHotStuffFile)


	return true;
}

// Picks a random song from the billboard100 file
cSong cMG_Imp::getRandomSong(void)
{
	cSong theSong;

	// Any songs loaded?
	if (this->numberOfAudiioInfoElements == 0)
	{
		return theSong;
	}

	
//	unsigned int randSongIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % this->getNumberOfSongsLoaded() ) - 1;
	unsigned int randSongIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % (this->getNumberOfSongsLoaded() - 1 ));

	// Which list of songs?
	if (randSongIndex < this->numberOfAudiioInfoElements)
	{
		// Use the audiio list

		// COPY (so de-reference) and return
		sAudiioCSVFileInfo& aSong = this->pAudiioInfoArray[randSongIndex];

		theSong.artist = aSong.performer;
		theSong.name = aSong.song;
		theSong.uniqueID = aSong.uniqueSongID;

		// At this point we DON'T know the song release date
		theSong.releaseDateYear = 0;
		theSong.releaseDateMonth = 0;
		theSong.releaseDateDay = 0;

		if (this->bChooseRandomDatesIfUknown)
		{
			this->assignRandomDate(theSong);
		}

		// Dates are in the "hot stuff" list
		// Did we load the "hot stuff" file?
		if (this->numberOfHotStuffElements > 0)
		{
			// Yes, so search for a possible match
			std::vector <cSong> vecMatchingSong;
			if ( this->findSong(theSong.name, theSong.artist, vecMatchingSong, false) )
			{
				// HACK: If more than one song, we're just picking the 1st one
				theSong.name = vecMatchingSong[0].name;
				theSong.artist = vecMatchingSong[0].artist;
				theSong.releaseDateYear = vecMatchingSong[0].releaseDateYear;
				theSong.releaseDateMonth = vecMatchingSong[0].releaseDateMonth;
				theSong.releaseDateDay = vecMatchingSong[0].releaseDateDay;
			}
			// If we didn't find a match, then the date had already set above
		}//if (this->numberOfHotStuffElements > 0)

	}
	else
	{
		// Pick a song from the "hot stuff" list

		// Offset index to start of "hot stuff" list
		// (index that was picked is from the sum of both audiio + hot_stuff)
		randSongIndex -= numberOfAudiioInfoElements;

		// COPY (so de-reference) and return
		sHotStuffCSVInfo& aSong = this->pHotStuffInfoArray[randSongIndex];

		theSong.artist = aSong.performer;
		theSong.name = aSong.song;
		theSong.uniqueID = aSong.uniqueSongID;
		theSong.releaseDateYear = aSong.weekid_Parsed.year;
		theSong.releaseDateMonth = aSong.weekid_Parsed.month;
		theSong.releaseDateDay = aSong.weekid_Parsed.day;

	}// if (randSongIndex


	return theSong;
}


// Makes up a random song (picks random artist + title, etc.)
// (i.e. the song doesn't exist)
cSong cMG_Imp::getRandomMadeUpSong(void) 
{
	cSong theSong;

	// Any songs loaded?
	if (this->numberOfAudiioInfoElements == 0 && this->numberOfHotStuffElements == 0)
	{
		// No songs, so nothing to pick from
		return theSong;
	}

	unsigned int randSongIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % this->getNumberOfSongsLoaded()) - 1;
	if (randSongIndex < this->numberOfAudiioInfoElements)
	{
		// Pick from audioo list
		theSong.artist = this->pAudiioInfoArray[randSongIndex].performer;
	}
	else
	{
		// Pick from "hot stuff" list
		randSongIndex -= this->numberOfAudiioInfoElements;
		theSong.artist = this->pHotStuffInfoArray[randSongIndex].performer;
	}

	randSongIndex = ((unsigned int)(this->p_theRandomizer->getNextRand()) % this->getNumberOfSongsLoaded()) - 1;
	if (randSongIndex < this->numberOfAudiioInfoElements)
	{
		// Pick from audioo list
		theSong.name = this->pAudiioInfoArray[randSongIndex].song;
	}
	else
	{
		// Pick from "hot stuff" list
		randSongIndex -= this->numberOfAudiioInfoElements;
		theSong.name = this->pHotStuffInfoArray[randSongIndex].song;
	}

	this->assignRandomDate(theSong);

	bool bSongID_OK = false;
	do
	{
		theSong.uniqueID = (this->p_theRandomizer->getNextRand() % ULONG_MAX) + 1;
		// Really unique?
		std::map<unsigned int, sUniqueSongInfo>::iterator itSongID = this->mapUniqueSongIDs_to_SongInfo.find(theSong.uniqueID);
		if (itSongID == this->mapUniqueSongIDs_to_SongInfo.end())
		{
			// Yup, it's unique, so add it
			sUniqueSongInfo songInfoEntry;
			songInfoEntry.pMadeUpSong = new cSong(theSong);
			this->mapUniqueSongIDs_to_SongInfo[theSong.uniqueID] = songInfoEntry;
			bSongID_OK = true;
		}
	} while (!bSongID_OK);

	// Update the look-up indices

	return theSong;
}

bool mapNameComparitor(std::string& a, std::string& b)
{
	if (a == b)
	{
		return true;
	}
	return false;
}

void cMG_Imp::copyAudiioInfo_to_cSong(const sAudiioCSVFileInfo& Ainfo, cSong& theSong)
{
	theSong.name = Ainfo.song;
	theSong.artist = Ainfo.performer;
	theSong.uniqueID = Ainfo.uniqueSongID;
	// No dates in the audiio info
	return;
}

void cMG_Imp::copyHotStuffInfo_to_cSong(const sHotStuffCSVInfo& HSinfo, cSong& theSong)
{
	theSong.name = HSinfo.song;
	theSong.artist = HSinfo.performer;
	theSong.uniqueID = HSinfo.uniqueSongID;
	theSong.releaseDateYear = HSinfo.weekid_Parsed.year;
	theSong.releaseDateMonth = HSinfo.weekid_Parsed.month;
	theSong.releaseDateDay = HSinfo.weekid_Parsed.day;
	return;
}

void cMG_Imp::copyUniqueSongInfo_to_cSong(const sUniqueSongInfo& USongInfo, cSong& theSong)
{
	if (USongInfo.pMadeUpSong != NULL)
	{
		cSong theSong;
		theSong = *(USongInfo.pMadeUpSong);
	}
	else if (USongInfo.pAudiioSong != NULL)
	{
		this->copyAudiioInfo_to_cSong(*(USongInfo.pAudiioSong), theSong);
	}
	else if (USongInfo.pHotStuffSong != NULL)
	{
		this->copyHotStuffInfo_to_cSong(*(USongInfo.pHotStuffSong), theSong);
	}
	return;
}


 
bool cMG_Imp::findSong(std::string songName, std::string artist, std::vector<cSong>& vecFoundSongs, bool bIgnoreCase)
{
	vecFoundSongs.clear();

	// Edge case: return ALL songs
	if (songName == "" && artist == "")
	{
		// Return everything
		for (const std::pair<unsigned int /*UniqueID*/, sUniqueSongInfo> &itSong : this->mapUniqueSongIDs_to_SongInfo)
		{
			cSong theSong;

			this->copyUniqueSongInfo_to_cSong(itSong.second, theSong);

			vecFoundSongs.push_back(theSong);

		}// for (...&itSong

		// EARLY exit
		return true;

	}//if (songName == "" && artist == "")




	// Return all songs by that artist
	if (songName == "")
	{
		if ( ! bIgnoreCase )
		{
			// Match case
			// One of few time "auto" might be OK outside a template... good lord...
			std::pair<std::multimap<std::string, unsigned int>::iterator, 
				      std::multimap<std::string, unsigned int>::iterator> 
				artistsSongs = this->map_Performer_to_UniqueID.equal_range(artist);
			
			for (std::multimap<std::string, unsigned int>::iterator itSong = artistsSongs.first; itSong != artistsSongs.second; itSong++)
			{
				cSong theSong;
				this->copyUniqueSongInfo_to_cSong(this->mapUniqueSongIDs_to_SongInfo[itSong->second], theSong);
				vecFoundSongs.push_back(theSong);
			}
		}
		else
		{
			// Ignore case
			for (char& curChar : artist)
			{
				curChar = toupper(curChar);
			}
			// One of few time "auto" might be OK outside a template... good lord...
			std::pair<std::multimap<std::string, unsigned int>::iterator,
				std::multimap<std::string, unsigned int>::iterator>
				artistsSongs = this->map_UpperCase_Performer_to_UniqueID.equal_range(artist);	// <-- Searching the UPPER case map

			for (std::multimap<std::string, unsigned int>::iterator itSong = artistsSongs.first; itSong != artistsSongs.second; itSong++)
			{
				cSong theSong;
				this->copyUniqueSongInfo_to_cSong(this->mapUniqueSongIDs_to_SongInfo[itSong->second], theSong);
				vecFoundSongs.push_back(theSong);
			}
		}

		// EARLY exit
		return true;

	}//if (songName == "")


	vecFoundSongs.clear();

	if ( ! bIgnoreCase )
	{
		// Use case matching original song names in file (case sensitive)

		std::string songIDtoFind = songName + artist;
		std::multimap<std::string, unsigned int>::iterator itSong = this->map_SongNamePlusPerformer_to_UniqueID.find(songIDtoFind);
		if (itSong == this->map_SongNamePlusPerformer_to_UniqueID.end())
		{
			// Didn't find anything
			return false;
		}

 		cSong theSong;
		this->copyUniqueSongInfo_to_cSong(this->mapUniqueSongIDs_to_SongInfo[itSong->second], theSong);
		vecFoundSongs.push_back(theSong);
		
		// Early exit
		return true;
	}
	else
	{
		// Ignore case
		for (char& curChar : songName)
		{
			curChar = toupper(curChar);
		}
		for (char& curChar : artist)
		{
			curChar = toupper(curChar);
		}

		std::string songIDtoFind = songName + artist;
		std::multimap<std::string, unsigned int>::iterator itSong = this->map_UpperCase_SongNamePlusPerformer_to_UniqueID.find(songIDtoFind);
		if (itSong == this->map_UpperCase_SongNamePlusPerformer_to_UniqueID.end())
		{
			// Didn't find anything
			return false;
		}

		cSong theSong;
		this->copyUniqueSongInfo_to_cSong(this->mapUniqueSongIDs_to_SongInfo[itSong->second], theSong);
		vecFoundSongs.push_back(theSong);

		// Early exit
		return true;

	}//if ( ! bIgnoreCase )


	return ( ! vecFoundSongs.empty() );
}


unsigned int cMG_Imp::getNumberOfSongsLoaded(void)
{
	unsigned int totalSongs = this->numberOfAudiioInfoElements;

	if (this->bLoadHotStuffFile)
	{
		totalSongs += this->numberOfHotStuffElements;
	}

	return totalSongs;
}


void cMG_Imp::assignRandomDate(cSong& theSong, unsigned long earliestYear /*=1920*/, unsigned long latestYear /*=2025*/)
{
	// TODO: We'll have to load the "hot_stuff.csv" to get the release date information
	theSong.releaseDateYear = (unsigned int)(this->p_theRandomizer->getRandBetween(earliestYear, latestYear));

	theSong.releaseDateMonth = (unsigned int)(this->p_theRandomizer->getRandBetween(1, 12));
	switch (theSong.releaseDateMonth)
	{
	case 1: // January
	case 3: // March
	case 5: // May
	case 7: // July
	case 8: // August
	case 10: // October
	case 12: // December
		theSong.releaseDateDay = (unsigned int)(this->p_theRandomizer->getRandBetween(1, 31));
		break;

	case 4: // April
	case 6: // June
	case 9: // September
	case 11: // November
		theSong.releaseDateDay = (unsigned int)(this->p_theRandomizer->getRandBetween(1, 30));
		break;

	case 2: // February
	{	// From Wikipedia: "leap day occurs in each year that is a multiple of 4, 
		//                  except for years evenly divisible by 100 but not by 400.
		//                  Thus 2000 and 2400 are leap years, but not 1700, 1800, 1900, 
		//                  2100, 2200, and 2300."
		//
		bool bIsLeapYear = false;
		// year that is a multiple of 4
		if (theSong.releaseDateYear % 4 == 0)
		{
			bIsLeapYear = true;
			// except for years evenly divisible by 100 but not by 400
			if ((theSong.releaseDateYear % 100 == 0) &&
				(theSong.releaseDateYear % 400 != 0))
			{
				bIsLeapYear = false;
			}
		}

		if (bIsLeapYear)
		{	// Leap year
			theSong.releaseDateDay = (unsigned int)(this->p_theRandomizer->getRandBetween(1, 29));
		}
		else
		{
			theSong.releaseDateDay = (unsigned int)(this->p_theRandomizer->getRandBetween(1, 28));
		}
	}
	break;

	}//switch (theSong.releaseDateMonth)

	return;
}
















// Doesn't work as the input file has some items with commas, BUT
//	those items are in double quotes
//	// Create stream for getline()
//std::stringstream ssFileData(std::string(pRawData, fileSize));
//
//unsigned int lineCount = 0;
//std::string theLine;
//while (std::getline(ssFileData, theLine))
//{
//	lineCount++;
//
//	// 1st line has header info, so skip
//	if (lineCount == 1)
//	{
//		continue;
//	}
//
//	std::stringstream ssLine(theLine);
//
//	sAudioCSVFileInfo audioEntryLine;
//
//	std::string token;
//	unsigned int tokenCount = 0;
//	while (std::getline(ssLine, token, ','))
//	{
//		switch (tokenCount)
//		{
//		case 0:		// std::string songid
//			audioEntryLine.songid = token;
//			break;
//		case 1:		// std::string performer
//			audioEntryLine.performer = token;
//			break;
//		case 2:		// std::string song
//			audioEntryLine.song = token;
//			break;
//		case 3:		// std::string spotify_genre
//			audioEntryLine.spotify_genre = token;
//			break;
//		case 4:		// std::string spotify_track_id
//			audioEntryLine.spotify_track_id = token;
//			break;
//		case 5:		// std::string spotify_track_preview_url
//			audioEntryLine.spotify_track_preview_url = token;
//			break;
//		case 6:		// std::string spotify_track_album
//			audioEntryLine.spotify_track_album = token;
//			break;
//		case 7:		// bool spotify_track_explicit
//			audioEntryLine.spotify_track_explicit = (token == "TRUE" ? true : false);
//			break;
//		case 8:		// unsigned int spotify_track_duration_ms
//			audioEntryLine.spotify_track_duration_ms = atoi(token.c_str());
//			break;
//		case 9:		// unsigned int spotify_track_popularity
//			audioEntryLine.spotify_track_popularity = atoi(token.c_str());
//			break;
//		case 10:	// float danceability
//			audioEntryLine.danceability = (float)atof(token.c_str());
//			break;
//		case 11:	// float energy
//			audioEntryLine.energy = (float)atof(token.c_str());
//			break;
//		case 12:	// unsigned int key
//			audioEntryLine.key = atoi(token.c_str());
//			break;
//		case 13:	// float loudness
//			audioEntryLine.loudness = (float)atof(token.c_str());
//			break;
//		case 14:	// bool mode
//			audioEntryLine.mode = (token == "TRUE" ? true : false);
//			break;
//		case 15:	// float speechiness
//			audioEntryLine.speechiness = (float)atof(token.c_str());
//			break;
//		case 16:	// double acousticness
//			audioEntryLine.acousticness = atof(token.c_str());
//			break;
//		case 17:	// double instrumentalness
//			audioEntryLine.instrumentalness = (float)atof(token.c_str());
//			break;
//		case 18:	// float liveness
//			audioEntryLine.liveness = (float)atof(token.c_str());
//			break;
//		case 19:	// float valence
//			audioEntryLine.valence = (float)atof(token.c_str());
//			break;
//		case 20:	// float tempo
//			audioEntryLine.tempo = (float)atof(token.c_str());
//			break;
//		case 21:	// unsigned int time_signature
//			audioEntryLine.time_signature = atoi(token.c_str());
//			break;
//		default:	// std::vector<std::string> vecExtraData
//			audioEntryLine.vecExtraData.push_back(token);
//		}//switch (tokenCount)
//
//		vecAudioInfo.push_back(audioEntryLine);
//
//		tokenCount++;
//	}//while (std::getline(ssLine, token, ','))
//}//while (std::getline(ssFileData, theLine))
//
//std::cout << "read " << lineCount << " lines" << std::endl;
//
//return true;
//}
