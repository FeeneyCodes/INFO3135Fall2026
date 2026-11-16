#ifndef _cMusicGenerator_Imp_HG_
#define _cMusicGenerator_Imp_HG_

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

#include <string>
#include <vector>
#include "cSong.h"
#include "cMersenneTwister.h"
#include <map>	// For the song IDs
#include "cCallbackProgress.h"


class cMG_Imp
{
public:
	cMG_Imp();
	~cMG_Imp();

	bool LoadMusicInformation(std::string musicFileName, std::string& errorString);

	// Picks a random song from the billboard100 file
	cSong getRandomSong(void);
	// Makes up a random song (picks random artist + title, etc.)
	// (i.e. the song doesn't exist)
	cSong getRandomMadeUpSong(void);


	// ******************************************************************
	// You don't need ANY of these for the INFO-3135 "Songify" project #1
	// ******************************************************************

	// Callback object for optional progress updates
	void setProgressCallbackObjectInstance(cCallbackProgress* pCallbackProgressObject);

	cCallbackProgress* pCallbackObject = NULL;

	// Called by this for updates
	void logProgressUpdate(std::string updateText);

	void assignRandomDate(cSong& theSong, unsigned long earliestYear = 1920, unsigned long latestYear = 2025);

	// These are based on the "audiio.csv" file in the billboard files
	// https://data.world/kcmillersean/billboard-hot-100-1958-2017
	struct sAudiioCSVFileInfo
	{
		std::string songid;
		std::string performer;
		std::string song;
		std::string spotify_genre;
		// Tokenized version of spotify_genre (without the square brackets)
		std::vector<std::string> vec_spotify_genres;	
		std::string spotify_track_id;
		std::string spotify_track_preview_url;
		std::string spotify_track_album;
		bool spotify_track_explicit;
		unsigned int spotify_track_duration_ms;
		unsigned int spotify_track_popularity;
		float danceability;
		float energy;
		unsigned int key;	
		float loudness;
		bool mode;
		float speechiness;
		double acousticness;		// Seems to have a very large number of decimal places
		double instrumentalness;	// Seems to have a very large number of decimal places
		float liveness;
		float valence;
		float tempo;
		unsigned int time_signature;
		// In case there's other columns?
		std::vector<std::string> vecExtraData;
		// 
		unsigned int uniqueSongID = 0;
	};
	void copyAudiioInfo_to_cSong(const sAudiioCSVFileInfo& Ainfo, cSong& theSong);


	// The goofly spelling of "Audio" is because the file has to "i"s in it.
	//
	// Note: this is c-style array so the entire thing can be allocated at once (faster)
	//	(i.e. instead of std::vector<sAudioCSVFileInfo*> vec_pAudioInfo;)
	sAudiioCSVFileInfo* pAudiioInfoArray = NULL;
	unsigned int numberOfAudiioInfoElements = 0;


	// This is based on the "hot_stuff_2" file in the billboard files
	// https://data.world/kcmillersean/billboard-hot-100-1958-2017
	struct sDate
	{
		unsigned int year;
		unsigned int month;
		unsigned int day;
	};
	struct sHotStuffCSVInfo
	{
		std::string url;
		std::string weekid;
		sDate weekid_Parsed;
		unsigned int week_position;
		std::string song;
		std::string performer;
		std::string songid;
		unsigned int instance;
		unsigned int previous_week_position;
		unsigned int peak_position;
		unsigned int weeks_on_chart;
		// In case there's other columns?
		std::vector<std::string> vecExtraData;
		// 
		unsigned int uniqueSongID = 0;
	};
	void copyHotStuffInfo_to_cSong(const sHotStuffCSVInfo& HSinfo, cSong& theSong);

	// Stored as pointers to make indexing easier
	// Note: this is c-style array so the entire thing can be allocated at once (faster)
	//	(i.e. instead of sstd::vector<sHotStuffCSVInfo*> vec_pHotStuffSongInfo;)
	sHotStuffCSVInfo* pHotStuffInfoArray = NULL;
	unsigned int numberOfHotStuffElements = 0;


	// This is used to make sure we have a unique song id
	struct sUniqueSongInfo
	{
		// Used for the "unique song ID" loop-up map
		// Whatever ISN'T NULL points to the song info
		sAudiioCSVFileInfo* pAudiioSong = NULL;
		sHotStuffCSVInfo* pHotStuffSong = NULL;
		// This is for non-existent, made up songs
		cSong* pMadeUpSong = NULL;
	};
	void copyUniqueSongInfo_to_cSong(const sUniqueSongInfo& USongInfo, cSong& theSong);
	// This map would contain EVERY song loaded or made up, indexed by UniqueID
	std::map<unsigned int /*UniqueID*/, sUniqueSongInfo> mapUniqueSongIDs_to_SongInfo;


	// Returns false if song isn't found.
	// TODO: This isn't complete, yet
	bool findSong(std::string songName, std::string artist, std::vector<cSong>& vecFoundSongs, bool bIgnoreCase);
	
	// For faster lookups
	// The "song_id" in the "hot_stuff_2" file is the song+performer
	// ex.
	// song: "Shake It Off"	
	// performer: "Taylor Swift"
	// songid: "Shake It OffTaylor Swift"
	std::multimap<std::string /*name+performer i.e. song_id*/, unsigned int /*UniqueID*/> map_SongNamePlusPerformer_to_UniqueID;
	std::multimap<std::string /*song*/, unsigned int /*UniqueID*/> map_SongName_to_UniqueID;
	std::multimap<std::string /*perfomer*/, unsigned int /*UniqueID*/> map_Performer_to_UniqueID;

	// These are the same, but in UPPER CASE, becuase you can't do a custom functor 
	//	or lambda on find() or equal_range() on multi-maps
	std::multimap<std::string /*name+performer i.e. song_id*/, unsigned int /*UniqueID*/> map_UpperCase_SongNamePlusPerformer_to_UniqueID;
	std::multimap<std::string /*song*/, unsigned int /*UniqueID*/> map_UpperCase_SongName_to_UniqueID;
	std::multimap<std::string /*perfomer*/, unsigned int /*UniqueID*/> map_UpperCase_Performer_to_UniqueID;


	bool bLoadHotStuffFile = false;
	bool bUseMultiThreadedParse = false;
	bool bChooseRandomDatesIfUknown = false;

	unsigned int getNumberOfSongsLoaded(void);

	cMersenneTwister* p_theRandomizer;	// = NULL;
};


#endif	// cMusicGenerator_Imp
