#pragma once

// This code is a variation from: https://en.wikipedia.org/wiki/Mersenne_Twister#Alternatives

class cMersenneTwister
{
public:
	cMersenneTwister(unsigned long seed);
	unsigned long getNextRand(void);
	//
	unsigned long getRandBetween(unsigned int lowest, unsigned int highest);
private:
	cMersenneTwister() {};	// Can't create without seed

	const static unsigned long n = 624;
	unsigned long m = 397;
	unsigned long w = 32;
	unsigned long r = 31;
	unsigned long UMASK = (0xffffffffUL << r);
	unsigned long LMASK = (0xffffffffUL >> (w - r));
	unsigned long a = 0x9908b0dfUL;
	unsigned long u = 11;
	unsigned long s = 7;
	unsigned long t = 15;
	unsigned long l = 18;
	unsigned long b = 0x9d2c5680UL;
	unsigned long c = 0xefc60000UL;
	unsigned long f = 1812433253UL;

	struct s_mt_state
	{
		unsigned long state_array[n] = { 0 };         // the array for the state vector 
		unsigned int state_index = 0;         // index into state vector array, 0 <= state_index <= n-1   always
	};
	s_mt_state mt_state;

	void m_initialize_state(unsigned long seed);
	unsigned long m_random_uint32(void);
};
