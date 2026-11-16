#ifndef _cLogger_HG_
#define _cLogger_HG_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

class cLogger
{
public:
	// If no name is give, it defaults to "LogFile"
	// The name always has the date and time, too.
	cLogger(std::string logFileName, bool b24HourFormat = true);
	~cLogger();

	void Log(std::string someText, bool bPrintTimeStamp = true);
	void Log(std::vector<std::string> vecSomeText, bool bPrintTimeStamp = true);
	// Prints a bunch of "-" characters
	void LogPrintLineBreak(unsigned int numHyphens = 20);

	// This is because the string stream clear is SO stupid
	// You call string appendedXX() until you're done, then call Log_OutputAppendedText()
	void appendText(std::string someText, bool bAddSpace = true);
	void appendNumber(int iNum, bool bAddSpace = true);
	void appendNumber(unsigned int uiNum, bool bAddSpace = true);
	void appendNumber(float fNum, bool bAddSpace = true);
	void appendNumber(double dNum, bool bAddSpace = true);
	void appendNumber(std::size_t stNum, bool bAddSpace = true);
	void Log_OutputAppendedText(bool bClearStringStream = true, bool bPrintTimeStamp = true);
private:
	std::stringstream m_ssAppendedTextStream;
public:


	bool bFlushAfterEveryLine = true;
	bool bAddBlankLineAfterEveryLog = false;
	bool bLogToScreenToo = true;
	bool bLogToErrorConsoleToo = false;
	bool bAddTimeToLogLines = true;
	// If bAddTimeToLogLines = false, this is ignored
	bool bAddDateToLogLines = false;
	// This can be different from the c'tor because
	//	we may want the file NAME to be 24 hour format,
	//	but each log entry as 12 hour (AM-PM) format, maybe?
	bool bUse24HourTime = true;

	// This is to see if the file was opened OK
	// Returns "OK" if no error
	std::string getFileStatus(void);
	// Returns false if in anything other than "good()" state
	bool bIsLogFileOK(void);
	std::string getLogFileFullName(void);



private:
	cLogger() {};

	// What was passed, if anything, in the constructor
	std::string m_logFileName;
	// The actual, full, file name that was used 
	// i.e. with date and time, etc. 
	std::string m_logFileFULLName;
	std::ofstream m_LogFile;

	std::string m_RemoveForbiddenFileCharacters(std::string fileName);

	std::string m_addLeadingZeros(unsigned int number, unsigned int numDigits = 2);

	// Returns the time() functions char array as a string
	// https://cplusplus.com/reference/ctime/ctime/
	// Www Mmm dd hh:mm:ss yyyy
	// example: "Wed Feb 13 16:06:10 2013"
	std::string m_get_time_AsString();
	struct sDateTime
	{
		// The time() functions char array as a string
		std::string rawTimeString;

		std::string dayAsReturned;
		std::string dayStringExtended;
		// 1 through 7, Sunday = 1
		unsigned int dayAsIntegerFromSunday = 1;

		std::string monthAsReturned;
		std::string monthStringExtended;
		// 1 through 12, January is 1
		unsigned int monthAsInteger = 0;

		unsigned int date = 0;

		unsigned int hourIn24HourFormat = 0;
		unsigned int hourIn12HourFormat = 0;
		bool bIsPM = false;
		unsigned int minute = 0;
		unsigned int second = 0;

		unsigned int year = 0;
	};
	void m_get_time_AsComponents(sDateTime& theDateAndTime);

	std::string m_getFullDateAndTime_ForLogLine(bool b24HourFormat = true);
	std::string m_getFullDateAndTime_ForLogLine(sDateTime& theTime, bool b24HourFormat = true);
	std::string m_getTime_ForLogLine(bool b24HourFormat = true);
	std::string m_getTime_ForLogLine(sDateTime& theTime, bool b24HourFormat = true);
};


#endif // cLogger
