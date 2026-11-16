#ifndef _sCanadianCityInfo_HG_
#define _sCanadianCityInfo_HG_

// Michael Feeney, 2025
// mfeeney (at) fanshawec.ca
//

#include <string>

struct sCanadianCityInfo
{
	std::string name;
	std::string province;
	unsigned long population = 0;
	unsigned long cumulative_population = 0;
};



#endif

