#include "cMusicGenerator.h"

#include "cMusicGenerator_Imp.h"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

cMusicGenerator::cMusicGenerator()
{
	this->p_Imp = new cMG_Imp();
#ifdef ALSO_LOAD_hot_stuff_2_FILE_FOR_DATES
	this->p_Imp->bLoadHotStuffFile = true;
#endif // ALSO_LOAD_hot_stuff_2_FILE_FOR_DATES

#ifdef USE_MULTITHREADED_FILE_PARSE
	this->p_Imp->bUseMultiThreadedParse = true;
#endif // USE_MULTITHREADED_FILE_PARSE

#ifdef CHOOSE_RANDOM_DATES_IF_HOT_STUFF_NOT_LOADED
	this->p_Imp->bChooseRandomDatesIfUknown = true;
#endif	// CHOOSE_RANDOM_DATES
}

cMusicGenerator::~cMusicGenerator()
{
	delete this->p_Imp;
}

bool cMusicGenerator::LoadMusicInformation(std::string musicFileName, std::string& errorString)
{

	return this->p_Imp->LoadMusicInformation(musicFileName, errorString);
}

cSong cMusicGenerator::cMusicGenerator::getRandomSong(void)
{
	return this->p_Imp->getRandomSong();
}

cSong cMusicGenerator::getRandomMadeUpSong(void)
{
	return this->p_Imp->getRandomMadeUpSong();
}

// Returns false if song isn't found.
// If bIgnoreCase=false, then this must be an EXACT MATCH for what's in the billboard100 file. 
// (So case sensitive, etc. )
bool cMusicGenerator::findSong(std::string songName, std::string artist, std::vector<cSong>& vecFoundSongs, bool bIgnoreCase /*=false*/)
{
	return this->p_Imp->findSong(songName, artist, vecFoundSongs, bIgnoreCase);
}

// Not part of student's interface
unsigned int cMusicGenerator::getNumberOfSongsLoaded(void)
{
	return (unsigned int)this->p_Imp->getNumberOfSongsLoaded();
}

#ifdef USE_CALLBACK_PROCESS_OBJECT
void cMusicGenerator::setProgressCallbackObjectInstance(cCallbackProgress* pCallbackProgressObject)
{
	this->p_Imp->setProgressCallbackObjectInstance(pCallbackProgressObject);
	return;
}
#endif
