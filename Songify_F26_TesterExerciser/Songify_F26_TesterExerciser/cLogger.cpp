#include "cLogger.h"
#include <sstream>
#include <iostream>
#include <time.h>	// or "<ctime>"

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

cLogger::cLogger(std::string logFileName, bool b24HourFormat /*=true*/)
{

	cLogger::sDateTime timeNow;
	this->m_get_time_AsComponents(timeNow);

	std::stringstream ssFileName;
	ssFileName
		<< this->m_RemoveForbiddenFileCharacters(this->m_getFullDateAndTime_ForLogLine(timeNow, b24HourFormat))
		<< " " << logFileName
		<< ".log";

	// Strip any characters we can't use in the file name
	this->m_logFileFULLName = this->m_RemoveForbiddenFileCharacters(ssFileName.str());

	this->m_LogFile.open(this->m_logFileFULLName.c_str());
	// if it DIDN'T open, then you can find this out 
	//	by calling bIsLogFileOK() or getFileStatus();

	if (this->m_LogFile.is_open())
	{
		this->m_LogFile
			<< "Log file >>" << ssFileName.str() << "<<" << '\n';
		if (this->m_logFileFULLName != ssFileName.str())
		{
			this->m_LogFile << "(actual filename is: >>" << this->m_logFileFULLName << "<<)\n";
		}

		this->m_LogFile << "opened : " << this->m_getFullDateAndTime_ForLogLine(timeNow, b24HourFormat) << '\n';
		this->m_LogFile << '\n';

		this->m_LogFile.flush();
	}

	// Note that if the file didn't open it can still output to cout (console screen) 

	this->bUse24HourTime = bUse24HourTime;
}

cLogger::~cLogger()
{
	if (this->m_LogFile.is_open())
	{
		cLogger::sDateTime timeNow;
		this->m_get_time_AsComponents(timeNow);

		this->m_LogFile << '\n';
		this->m_LogFile << "closed : " << this->m_getFullDateAndTime_ForLogLine(timeNow, this->bUse24HourTime) << '\n';

		this->m_LogFile.flush();

		this->m_LogFile.close();
	}
}

// Prints a bunch of "-" characters
void cLogger::LogPrintLineBreak(unsigned int numHyphens /*=20*/)
{
	const std::string aLongLineOfHyphens
		= "--------------------------------------------------------------------------------";

	// Want more than aLongLineOfHyphens long? (like 80)
	// (Really? What's wrong with you?)
	if (numHyphens > aLongLineOfHyphens.size())
	{
		// Make a really long line of hyphens, then...
		std::stringstream ssReallyLongLine;
		for (unsigned int count = 0; count != numHyphens; count++)
		{
			ssReallyLongLine << "-";
		}
		this->Log(ssReallyLongLine.str());
	}
	// It's under 80 hypens
	this->Log(aLongLineOfHyphens.substr(0, numHyphens));
	return;
}


std::string cLogger::m_RemoveForbiddenFileCharacters(std::string fileName)
{
	// According to: https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
	//	The following reserved characters:
	//		< (less than)
	//		> (greater than)
	//		: (colon)
	//		" (double quote)
	//		/ (forward slash)
	//		\ (backslash)
	//		| (vertical bar or pipe)
	//		? (question mark)
	//		* (asterisk)

	std::stringstream ssGoodFileName;

	for (char& theChar : fileName)
	{
		switch (theChar)
		{
		case '<':	// (less than)
			// https://en.wikipedia.org/wiki/Less-than_sign
			// U+FF1C or U+65308 "＜" Fullwidth Less-than sign
			//ssGoodFileName << "＜";
			// If your code-page can't deal with it...
			ssGoodFileName << "&lt;";
			break;
		case '>':	// (greater than)
			// https://en.wikipedia.org/wiki/Greater-than_sign
			// U+FF1E or U+65310 "＞" Fullwidth Greater-than sign
			//ssGoodFileName << "＞";
			// If your code-page can't deal with it...
			ssGoodFileName << "&gt;";
			break;
		case ':':	// (colon)
			// https://en.wikipedia.org/wiki/Colon_(punctuation)
			// U+2236 "∶" mathematical "ratio" character
			//ssGoodFileName << "∶";
			// If your code-page can't deal with it...
			ssGoodFileName << "-";
			break;
		case '"':	// (double quote)
			// https://en.wikipedia.org/wiki/Quotation_mark#Unicode_code_point_table
			// U+FF02 or U-65282 "＂" Full-width quotation mark
			//ssGoodFileName << "＂";
			// If your code-page can't deal with it...
			ssGoodFileName << "'";	// Replace with single quote
			break;
		case '/':	// (forward slash or a "Solidus")
			// https://en.wikipedia.org/wiki/Slash_(punctuation)
			// U+2044 or U-8260 "⁄" Fractional slash
			//ssGoodFileName << "⁄";
			// If your code-page can't deal with it...
			ssGoodFileName << "_";
			break;
		case '\\':	// (backslash or a "reverse solidus")
			// https://en.wikipedia.org/wiki/Backslash
			// U+29F5 or U-10741 "⧵" Reverse Solidus operator
			//ssGoodFileName << "⧵";
			// If your code-page can't deal with it...
			ssGoodFileName << "_";
			break;
		case '|':	// (vertical bar or pipe)
			// https://en.wikipedia.org/wiki/Vertical_bar
			// U+FF5C or U-65372 "｜" Full-width vertical line
			//ssGoodFileName << "｜";
			// If your code-page can't deal with it...
			ssGoodFileName << "-";
			break;
		case '?':	// (question mark)
			// https://en.wikipedia.org/wiki/Question_mark#Computing
			// U+FF1F or U-65311 "？" Full-width question mark
			//ssGoodFileName << "？";
			// If your code-page can't deal with it...
			ssGoodFileName << " ";
			break;
		case '*':	// (asterisk)
			// https://en.wikipedia.org/wiki/Asterisk
			// U+2055 or U-8277 "⁕" Flower punctuation mark
			//ssGoodFileName << "⁕";
			// If your code-page can't deal with it...
			ssGoodFileName << "+";
			break;
			// 
		default:
			// Character is A-OK
			ssGoodFileName << theChar;
		}//switch (theChar)
	}//for ( char &theChar : fileName )

	return ssGoodFileName.str();
}


void cLogger::Log(std::string someText, bool bPrintTimeStamp /*=true*/)
{
	std::stringstream ssTheLine;

	if (bPrintTimeStamp)
	{
		cLogger::sDateTime timeNow;
		this->m_get_time_AsComponents(timeNow);

		if (this->bAddTimeToLogLines)
		{
			// Date, too?
			if (this->bAddDateToLogLines)
			{
				ssTheLine << this->m_getFullDateAndTime_ForLogLine(timeNow, this->bUse24HourTime);
				ssTheLine << " @";
			}
			//
			ssTheLine << this->m_getTime_ForLogLine(timeNow, this->bUse24HourTime);
		}//if ( this->bAddTimeToLogLines )

		ssTheLine << " : ";

	}//if (bPrintTimeStamp)

	ssTheLine << someText << '\n';

	if (this->bAddBlankLineAfterEveryLog)
	{
		ssTheLine << '\n';
	}

	this->m_LogFile << ssTheLine.str();

	if (this->bFlushAfterEveryLine)
	{
		this->m_LogFile.flush();
	}

	if (this->bLogToScreenToo)
	{
		std::cout << ssTheLine.str();
		if (this->bFlushAfterEveryLine)
		{
			std::cout.flush();
		}
	}

	if (this->bLogToErrorConsoleToo)
	{
		std::cerr << ssTheLine.str();
	}

	return;
}

void cLogger::Log(std::vector<std::string> vecSomeText, bool bPrintTimeStamp /*=true*/)
{
	for (std::string& curLine : vecSomeText)
	{
		this->Log(curLine, bPrintTimeStamp);
	}
	return;
}




std::string cLogger::m_getFullDateAndTime_ForLogLine(bool b24HourFormat /*=true*/)
{
	cLogger::sDateTime timeNow;
	this->m_get_time_AsComponents(timeNow);
	return this->m_getFullDateAndTime_ForLogLine(timeNow, b24HourFormat);
}

std::string cLogger::m_addLeadingZeros(unsigned int number, unsigned int numDigits /*=2*/)
{
	std::stringstream ssDigits;
	ssDigits << number;
	if (ssDigits.str().length() < numDigits)
	{
		unsigned int digitsToAdd = numDigits - (unsigned int)ssDigits.str().length();
		// Clear it (good lord, stringstream clear is so stupid...)
		ssDigits.str("");
		ssDigits.clear();
		for (unsigned int count = 0; count != digitsToAdd; count++)
		{
			ssDigits << "0";
		}
		ssDigits << number;
	}
	return ssDigits.str();
}



std::string cLogger::m_getFullDateAndTime_ForLogLine(sDateTime& theTime, bool b24HourFormat /*=true*/)
{

	std::stringstream ssTime;
	if (b24HourFormat)
	{
		ssTime
			<< this->m_addLeadingZeros(theTime.hourIn24HourFormat, 2) << ":"
			<< this->m_addLeadingZeros(theTime.minute, 2) << ":"
			<< this->m_addLeadingZeros(theTime.second, 2);
	}
	else // 12 hour format, AM-PM
	{
		ssTime
			<< theTime.hourIn12HourFormat << ":"
			<< this->m_addLeadingZeros(theTime.minute, 2) << ":"
			<< this->m_addLeadingZeros(theTime.second, 2) << " "
			<< (theTime.bIsPM ? "PM" : "AM");
	}//if ( b24HourFormat )

	std::stringstream ssDateNumbers;
	ssDateNumbers
		<< theTime.year << "-"
		<< this->m_addLeadingZeros(theTime.monthAsInteger, 2) << "-"
		<< this->m_addLeadingZeros(theTime.date, 2);

	std::stringstream ssDateText;
	ssDateText
		<< theTime.dayStringExtended << ", "
		<< theTime.monthStringExtended << " "
		<< theTime.date << ", "
		<< theTime.year;

	std::stringstream ssEverything;
	ssEverything
		<< ssDateNumbers.str() << " "
		<< "[" << ssTime.str() << "] "
		<< "(" << ssDateText.str() << ")";


	return ssEverything.str();
}

std::string cLogger::m_getTime_ForLogLine(bool b24HourFormat /*=true*/)
{
	cLogger::sDateTime timeNow;
	this->m_get_time_AsComponents(timeNow);
	return this->m_getTime_ForLogLine(timeNow, b24HourFormat);
}

std::string cLogger::m_getTime_ForLogLine(sDateTime& theTime, bool b24HourFormat /*=true*/)
{

	std::stringstream ssTime;
	if (b24HourFormat)
	{
		ssTime
			<< this->m_addLeadingZeros(theTime.hourIn24HourFormat, 2) << ":"
			<< this->m_addLeadingZeros(theTime.minute, 2) << ":"
			<< this->m_addLeadingZeros(theTime.second, 2);
	}
	else // 12 hour format, AM-PM
	{
		ssTime
			<< theTime.hourIn12HourFormat << ":"
			<< this->m_addLeadingZeros(theTime.minute, 2) << ":"
			<< this->m_addLeadingZeros(theTime.second, 2) << " "
			<< (theTime.bIsPM ? "PM" : "AM");
	}//if ( b24HourFormat )

	return ssTime.str();;
}

// Returns the time() functions char array as a string
std::string cLogger::m_get_time_AsString()
{
	const unsigned int BUFFER_SIZE = 256;
	time_t timeNow;
	time(&timeNow);
	char strBuffer[BUFFER_SIZE] = { 0 };
	ctime_s(strBuffer, BUFFER_SIZE, &timeNow);
	std::string sTime(strBuffer);

	return sTime;
}

void cLogger::m_get_time_AsComponents(sDateTime& theDateAndTime)
{
	theDateAndTime.rawTimeString = this->m_get_time_AsString();

	// https://cplusplus.com/reference/ctime/ctime/
	// returned in this format: Www Mmm dd hh:mm:ss yyyy
	// example: "Wed Feb 13 16:06:10 2013"

	std::size_t firstColon = theDateAndTime.rawTimeString.find(':');
	std::size_t secondColon = theDateAndTime.rawTimeString.find(':', firstColon + 1);

	theDateAndTime.dayAsReturned = theDateAndTime.rawTimeString.substr(0, 3);
#pragma region ProcessTheDate
	// 1 through 7, Sunday = 1
	// https://en.cppreference.com/w/cpp/chrono/c/ctime.html
	// "Www - the day of the week (one of Mon, Tue, Wed, Thu, Fri, Sat, Sun)."
	if (theDateAndTime.dayAsReturned == "Sun")
	{
		theDateAndTime.dayStringExtended = "Sunday";
		theDateAndTime.dayAsIntegerFromSunday = 1;
	}
	else if (theDateAndTime.dayAsReturned == "Mon")
	{
		theDateAndTime.dayStringExtended = "Monday";
		theDateAndTime.dayAsIntegerFromSunday = 2;
	}
	else if (theDateAndTime.dayAsReturned == "Tue")
	{
		theDateAndTime.dayStringExtended = "Tuesday";
		theDateAndTime.dayAsIntegerFromSunday = 3;
	}
	else if (theDateAndTime.dayAsReturned == "Wed")
	{
		theDateAndTime.dayStringExtended = "Wednesday";
		theDateAndTime.dayAsIntegerFromSunday = 4;
	}
	else if (theDateAndTime.dayAsReturned == "Thu")
	{
		theDateAndTime.dayStringExtended = "Thursday";
		theDateAndTime.dayAsIntegerFromSunday = 5;
	}
	else if (theDateAndTime.dayAsReturned == "Fri")
	{
		theDateAndTime.dayStringExtended = "Friday";
		theDateAndTime.dayAsIntegerFromSunday = 6;
	}
	else if (theDateAndTime.dayAsReturned == "Sat")
	{
		theDateAndTime.dayStringExtended = "Saturday";
		theDateAndTime.dayAsIntegerFromSunday = 7;
	}
	//	else it'll be the default values ("" and 0)
#pragma endregion

	theDateAndTime.monthAsReturned = theDateAndTime.rawTimeString.substr(4, 3);
#pragma region ProcessTheMonth
	// 1 through 12, January is 1
	// https://en.cppreference.com/w/cpp/chrono/c/ctime.html
	// "Mmm - the month (one of Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec)."
	if (theDateAndTime.monthAsReturned == "Jan")
	{
		theDateAndTime.monthStringExtended = "January";
		theDateAndTime.monthAsInteger = 1;
	}
	else if (theDateAndTime.monthAsReturned == "Feb")
	{
		theDateAndTime.monthStringExtended = "February";
		theDateAndTime.monthAsInteger = 2;
	}
	else if (theDateAndTime.monthAsReturned == "Mar")
	{
		theDateAndTime.monthStringExtended = "March";
		theDateAndTime.monthAsInteger = 3;
	}
	else if (theDateAndTime.monthAsReturned == "Apr")
	{
		theDateAndTime.monthStringExtended = "April";
		theDateAndTime.monthAsInteger = 4;
	}
	else if (theDateAndTime.monthAsReturned == "May")
	{
		theDateAndTime.monthStringExtended = "May";
		theDateAndTime.monthAsInteger = 5;
	}
	else if (theDateAndTime.monthAsReturned == "Jun")
	{
		theDateAndTime.monthStringExtended = "June";
		theDateAndTime.monthAsInteger = 6;
	}
	else if (theDateAndTime.monthAsReturned == "Jul")
	{
		theDateAndTime.monthStringExtended = "Juny";
		theDateAndTime.monthAsInteger = 7;
	}
	else if (theDateAndTime.monthAsReturned == "Aug")
	{
		theDateAndTime.monthStringExtended = "August";
		theDateAndTime.monthAsInteger = 8;
	}
	else if (theDateAndTime.monthAsReturned == "Sep")
	{
		theDateAndTime.monthStringExtended = "September";
		theDateAndTime.monthAsInteger = 9;
	}
	else if (theDateAndTime.monthAsReturned == "Oct")
	{
		theDateAndTime.monthStringExtended = "October";
		theDateAndTime.monthAsInteger = 10;
	}
	else if (theDateAndTime.monthAsReturned == "Nov")
	{
		theDateAndTime.monthStringExtended = "November";
		theDateAndTime.monthAsInteger = 11;
	}
	else if (theDateAndTime.monthAsReturned == "Dec")
	{
		theDateAndTime.monthStringExtended = "December";
		theDateAndTime.monthAsInteger = 12;
	}
	//	else it'll be the default values ("" and 0)
#pragma endregion

	theDateAndTime.date = atoi(theDateAndTime.rawTimeString.substr(8, 2).c_str());
	theDateAndTime.year = atoi(theDateAndTime.rawTimeString.substr(20, 4).c_str());


	theDateAndTime.hourIn24HourFormat = atoi(theDateAndTime.rawTimeString.substr(firstColon - 2, 2).c_str());
	theDateAndTime.minute = atoi(theDateAndTime.rawTimeString.substr(firstColon + 1, 2).c_str());
	theDateAndTime.second = atoi(theDateAndTime.rawTimeString.substr(secondColon + 1, 2).c_str());

	// The format gets returned as 24 hours
	theDateAndTime.hourIn12HourFormat = theDateAndTime.hourIn24HourFormat;
	if (theDateAndTime.hourIn24HourFormat > 12)
	{
		theDateAndTime.bIsPM = true;
		theDateAndTime.hourIn12HourFormat = theDateAndTime.hourIn24HourFormat - 12;
	}

	return;
}






std::string cLogger::getFileStatus(void)
{
	if (this->bIsLogFileOK())
	{
		return "OK";
	}
	// Something is wrong
	std::stringstream ssError;
	ssError << "Log file [" << this->m_logFileFULLName << "] state: ";

	if (this->m_LogFile.bad())
	{
		ssError << "badbit set ";
	}
	if (this->m_LogFile.fail())
	{
		ssError << "failbit set ";
	}
	if (this->m_LogFile.eof())
	{
		ssError << "eofbit set ";
	}

	ssError << (this->m_LogFile.is_open() ? " file is open " : " file is closed ");

	return ssError.str();
}

bool cLogger::bIsLogFileOK(void)
{
	return this->m_LogFile.good();
}

std::string cLogger::getLogFileFullName(void)
{
	return this->m_logFileFULLName;
}



// You call string appendedXX() until you're done, then call Log_OutputAppendedText()
void cLogger::appendText(std::string someText, bool bAddSpace /*=true*/)
{
	this->m_ssAppendedTextStream << someText;
	if (bAddSpace)
	{
		this->m_ssAppendedTextStream << " ";
	}
	return;
}

void cLogger::appendNumber(int iNum, bool bAddSpace /*=true*/)
{
	this->m_ssAppendedTextStream << iNum;
	if (bAddSpace)
	{
		this->m_ssAppendedTextStream << " ";
	}
	return;
}

void cLogger::appendNumber(unsigned int uiNum, bool bAddSpace /*=true*/)
{
	this->m_ssAppendedTextStream << uiNum;
	if (bAddSpace)
	{
		this->m_ssAppendedTextStream << " ";
	}
	return;
}

void cLogger::appendNumber(std::size_t stNum, bool bAddSpace /*=true*/)
{
	this->appendNumber((unsigned int)stNum, bAddSpace);
	return;
}

void cLogger::appendNumber(float fNum, bool bAddSpace /*=true*/)
{
	this->m_ssAppendedTextStream << fNum;
	if (bAddSpace)
	{
		this->m_ssAppendedTextStream << " ";
	}
	return;
}

void cLogger::appendNumber(double dNum, bool bAddSpace /*=true*/)
{
	this->m_ssAppendedTextStream << dNum;
	if (bAddSpace)
	{
		this->m_ssAppendedTextStream << " ";
	}
	return;
}

void cLogger::Log_OutputAppendedText(bool bClearStringStream /*=true*/, bool bPrintTimeStamp /*=true*/)
{
	this->Log(this->m_ssAppendedTextStream.str(), bPrintTimeStamp);
	// My gawd the string stream clear() is so fucking stupid. Honestly, fix the damn thing.
	this->m_ssAppendedTextStream.str("");
	this->m_ssAppendedTextStream.clear();
	return;
}

