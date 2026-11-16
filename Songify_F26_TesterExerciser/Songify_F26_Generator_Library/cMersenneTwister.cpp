#include "cMersenneTwister.h"

cMersenneTwister::cMersenneTwister(unsigned long seed)
{
    this->m_initialize_state(seed);
}

unsigned long cMersenneTwister::getNextRand(void)
{
    return this->m_random_uint32();
}

unsigned long cMersenneTwister::getRandBetween(unsigned int lowest, unsigned int highest)
{
    return (this->getNextRand() % (highest - lowest)) + lowest;
}

void cMersenneTwister::m_initialize_state(/*s_mt_state* state, */unsigned long seed)
{
    //unsigned long* state_array = &(state->state_array[0]);
    unsigned long* state_array = &(this->mt_state.state_array[0]);

    state_array[0] = seed;                          // suggested initial seed = 19650218UL

    for (int i = 1; i < n; i++)
    {
        seed = f * (seed ^ (seed >> (w - 2))) + i;    // Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
        state_array[i] = seed;
    }

    this->mt_state.state_index = 0;

    return;
}

//unsigned long cMersenneTwister::m_random_uint32(s_mt_state* state)
unsigned long cMersenneTwister::m_random_uint32(void)
{
    //unsigned long* state_array = &(state->state_array[0]);
    unsigned long* state_array = &(this->mt_state.state_array[0]);

    int k = this->mt_state.state_index; // point to current state location
    // 0 <= state_index <= n-1   always

//  int k = k - n;                   // point to state n iterations before
//  if (k < 0) k += n;               // modulo n circular indexing
                                     // the previous 2 lines actually do nothing
                                     //  for illustration only

    int j = k - (n - 1);               // point to state n-1 iterations before
    if (j < 0) j += n;               // modulo n circular indexing

    unsigned long x = (state_array[k] & UMASK) | (state_array[j] & LMASK);

    unsigned long xA = x >> 1;
    if (x & 0x00000001UL) xA ^= a;

    j = k - (n - m);                   // point to state n-m iterations before
    if (j < 0) j += n;               // modulo n circular indexing

    x = state_array[j] ^ xA;         // compute next value in the state
    state_array[k++] = x;            // update new state value

    if (k >= n) k = 0;               // modulo n circular indexing
    this->mt_state.state_index = k;

    unsigned long y = x ^ (x >> u);       // tempering 
    y = y ^ ((y << s) & b);
    y = y ^ ((y << t) & c);
    unsigned long z = y ^ (y >> l);

    return z;
}