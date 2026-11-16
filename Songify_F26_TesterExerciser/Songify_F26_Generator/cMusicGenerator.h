#ifndef _cMusicGenerator_HG_
#define _cMusicGenerator_HG_

#include "cSong.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

// Forward define for implementation class
class cMG_Imp;

// The dates are pulled from a separate file (hot_stuff_2.cvs)
// ...but it's MUCH larger so takes a while to load
//
// Uncomment this if you want this file loaded, too
//#define ALSO_LOAD_hot_stuff_2_FILE_FOR_DATES
// 
// This will greatly speed up the loading of the file
#define USE_MULTITHREADED_FILE_PARSE
//
// The songs in the audiio files don't have dates. 
// (The "hot stuff" file does contain dates)
// Uncomment this if you want it to pick random dates for the audiio files,
//	otherwise those dates will stay all zeros (year, month, and day = 0)
// If you DO load the "hot_stuff_2" file, the correct dates will be taken
//	from and song matches found (song name + artist)
// i.e. this only matters if you DON'T load the giant "hot stuff" file.
#define CHOOSE_RANDOM_DATES_IF_HOT_STUFF_NOT_LOADED


//#define USE_CALLBACK_PROCESS_OBJECT

#ifdef USE_CALLBACK_PROCESS_OBJECT
// Used by the callback updater
#include "cCallbackProgress.h"
#endif 

#include <vector>

class cMusicGenerator
{
public:
	cMusicGenerator();
	~cMusicGenerator();

	// If "musicFileName" is blank, it'll assume it's "audiio.csv" 
	bool LoadMusicInformation(std::string musicFileName, std::string& errorString);

	// Picks a random song from the billboard100 file
	cSong getRandomSong(void);
	// Makes up a random song (picks random artist + title, etc.)
	// (i.e. the song doesn't exist)
	cSong getRandomMadeUpSong(void);



	// ******************************************************************
	// You don't need ANY of these for the INFO-3135 "Songify" project #1
	// ******************************************************************

	// TODO: Not complete
	// 
	// Returns false if song isn't found.
	// If bIgnoreCase=false, then this must be an EXACT MATCH for what's in the billboard100 file. 
	// (So case sensitive, etc. )
	// If only songName present, returns all songs with that name.
	// If only artist present, returns all songs by that artist.
	//   (it ignores collaborations or "featuring")
	// If both blank, returns all songs.
	//
	// This is just meant for looking up songs you know you loaded;
	//	it's NOT some comprehensive "google" of REGEX of searches.
	//
	bool findSong(std::string songName, std::string artist, std::vector<cSong> &vecFoundSongs, bool bIgnoreCase=false);

	unsigned int getNumberOfSongsLoaded(void);

#ifdef USE_CALLBACK_PROCESS_OBJECT
	// Callback object for optional progress updates
	void setProgressCallbackObjectInstance(cCallbackProgress* pCallbackProgressObject);
#endif
private:
	cMG_Imp* p_Imp = NULL;
};

#endif
