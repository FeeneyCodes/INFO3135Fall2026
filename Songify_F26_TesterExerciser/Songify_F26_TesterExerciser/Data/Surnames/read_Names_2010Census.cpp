#include <iostream>
#include <fstream>
#include <sstream>		// String Stream
#include <string>

int main()
{
	// Open the file
	std::ifstream namesFile("Names_2010Census.csv");
	if ( ! namesFile.is_open() )
	{
		std::cout << "Didn't open file" << std::endl;
		return -1;
	}

// name,rank,count,prop100k,cum_prop100k,pctwhite,pctblack,pctapi,pctaian,pct2prace,pcthispanic
// SMITH,1,2442977,828.19,828.19,70.9,23.11,0.5,0.89,2.19,2.4
	
	std::string theLine;

	unsigned int lineCount = 0;
	while (std::getline(namesFile, theLine))
	{
		lineCount++;
		std::stringstream ssLine(theLine);

		std::string token;
		unsigned int tokenCount = 0;
		while (std::getline(ssLine, token, ','))
		{
			if ( tokenCount == 0 )
			{
				std::cout << token << std::endl;
			}
			// Ignore the other parts of the line
			tokenCount++;
		}
	}

	std::cout << "Lines read = " << lineCount << std::endl;
	
	return 0;
}

