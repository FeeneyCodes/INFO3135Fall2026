#ifndef _cSong_HG_
#define _cSong_HG_

#include <string>

// This represents a song in the Snotify class

class cSong
{
public:
	cSong()
	{
		this->rating = 0;
		this->numberOfTimesPlayed = 0;
		this->releaseDateDay = 0;
		this->releaseDateMonth = 0;
		this->releaseDateYear = 0;
	}
	std::string name;
	std::string artist;
	unsigned int releaseDateDay;
	unsigned int releaseDateMonth;
	unsigned int releaseDateYear;

	// These are used just for user play statistics
	int rating;					// rating, by the USER, from 0 to 5 (5 is best)
	int numberOfTimesPlayed;	// number of times the USER has listened to this song

	// You can implement this any way you'd like, but I will be using 
	//	this method rather than the public variable. 
	// The cPerson has a common method for generating unique IDs, if you 
	//	want to use that. 
	// The "music generator" will provide this 
	unsigned int getUniqueID(void) { return this->uniqueID; }

	// This would contain the data for the song that we would be playing
	// This would be given to you from the "music generator"
	char* pSongData = NULL;

	unsigned int uniqueID = 0;
}; 

#endif

