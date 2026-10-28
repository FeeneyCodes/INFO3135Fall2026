#ifndef _cSongify_HG_
#define _cSongify_HG_

#include "cPerson.h"
#include "cSong.h"


class cSongify
{
public:
	cSongify();
	~cSongify();

	// All these return true if they work, false if not (for any reason)
	// You can add some error information to the by referenece error string, if you want. 
	bool AddUser(cPerson newPerson, std::string& errorString);

	// Returns false if not found
	bool FindUser(unsigned int SongifyID, cPerson& foundPerson);
	// Returns false if not found OR there's more than one user (which shouldn't happen, anyway...)
	bool FindUserBySIN(unsigned int SIN, cPerson& foundPerson);
	bool FindUserBySongifyID(unsigned int SongifyID, cPerson& foundPerson);


	// This will ++OVERWRITE++ the data contained in user 
	// 
	// The idea is that you'd call "FindUser" then change just what needs to be
	//	changed, then call this method - thereby updating what got 
	// 
	// NOTE: It "finds" the user by matching BOTH the "unique Songify ID" AND the "SIN" 
	//        (Social Insurance Number)
	// So if either of these DON'T match (or aren't found) then it reutrns false. 
	// 
	// Returns false if it can't find the person
	bool UpdateUser(cPerson& thePerson, std::string& errorString);
	// Returns false if it can't find the person
	bool DeleteUser(unsigned int SongifyUserID, std::string& errorString);

	// Returns false if these don't work
	// 
	// ---------------------------------------------------------------------
	// This returns a ++COPY++ of the users library. 
	// The CALLER will delete this data when done using it.
	// i.e. you will have to allocate and copy the library into this array 
	// ---------------------------------------------------------------------
	// 
	// See the cSongify.cpp file for more infomration on how this is supposed to work
	// 
	// It will return false if it can't find that user
	bool GetUsersSongLibrary(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);
	// Song array is sorted by this user's rating, from HIGHEST to lowest
	bool GetUsersSongLibraryAscendingBySongRating(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);
	// Song array is sorted by HIGHEST number of plays to lowest
	bool GetUsersSongLibraryAscendingByNumberOfPlays(unsigned int songifyUserID, cSong*& pLibraryArray, unsigned int& sizeOfLibary);

	// If sortAscending = true, then returns ascending sort by LAST then FIRST names
	// (false returns descending)
	bool GetUsers(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray, bool sortAscending);
	// Sorded ASCENDING by Songify ID
	bool GetUsersByID(cPerson*& pAllTheUsers, unsigned int& sizeOfUserArray);


	// If an identical song is added, it's ignored.
	bool AddSong(cSong newSong, std::string& errorString);

	// This finds a match, based on the "unique ID", and overwrites the data. 
	bool UpdateSong(cSong theSong, std::string& errorString);

	// ************************************************************************
	// NOTE: Deleting the song deletes it from ALL users that have this song!!!
	// ************************************************************************
	bool DeleteSong(unsigned int UniqueSongID, std::string& errorString);

	// This associates a particular song with a particular user. 
	// (Note: returns true if the song is already there)
	bool AddSongToUserLibrary(unsigned int songifyUserID, unsigned int UniqueSongID, std::string& errorString);
	bool RemoveSongFromUserLibrary(unsigned int songifyUserID, unsigned int SongifySongID, std::string& errorString);

	// Note: this is unique TO THE USER, not the entire Songify system.
	// i.e. each user has a different rating for each song.
	bool UpdateRatingOnSong(unsigned int SongifyUserID, unsigned int songUniqueID, unsigned int newRating);

	// Returns 0, NULL, or nullptr if you can't find the song.
	// (Can return optional information on errorString)
	// 
	// ****************************************************************************
	// NOTE: This updates the "numberOfTimesPlayed" in THIS USER'S playlist-library
	// ****************************************************************************
	bool GetSongToPlay(unsigned int SongifyUserID, unsigned int songUniqueID, cSong& foundSong, std::string& errorString);
	
	// Note that the songRating is "returned" by reference. 
	// If it can't find a match (i.e. returns false), then the rating isn't valid. 
	bool GetCurrentSongRating(unsigned int songifyUserID, unsigned int songUniqueID, unsigned int &songRating);
	bool GetCurrentSongNumberOfPlays(unsigned int songifyUserID, unsigned int songUniqueID, unsigned int &numberOfPlays);

	// This isn't tied to a particular user, so doesn't update any stats
	// (You can also assume that with duplicates, it will return the 1st one it finds. I don't care which.)
	bool FindSong(std::string title, std::string artist, cSong& foundSong);
	bool FindSong(unsigned int uniqueID, cSong& foundSong);


};

#endif // !_cSongify_HG_
