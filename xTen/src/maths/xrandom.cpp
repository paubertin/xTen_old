#include "xrandom.h"
#include <time.h>

namespace xten { namespace xmaths {

	xRandom::xRandom() :
		seed(1), seed_sp(0), mti(XMATH_N+1)
	{
	}

	unsigned int xRandom::random(unsigned int n)
	{
		unsigned long y;
		static unsigned long mag01[2] = { 0x0, XMATH_MATRIX_A };

		if (n == 0)
			return(0);

		/* mag01[x] = x * MATRIX_A  for x=0,1 */

		if (mti >= XMATH_N) { /* generate N words at one time */
			int kk;

			if (mti == XMATH_N + 1)   /* if sgenrand() has not been called, */
				setRandomSeed(4357); /* a default initial seed is used   */

			for (kk = 0; kk<XMATH_N - XMATH_M; kk++) {
				y = (mt[kk] & XMATH_UPPER_MASK) | (mt[kk + 1] & XMATH_LOWER_MASK);
				mt[kk] = mt[kk + XMATH_M] ^ (y >> 1) ^ mag01[y & 0x1];
			}
			for (; kk<XMATH_N - 1; kk++) {
				y = (mt[kk] & XMATH_UPPER_MASK) | (mt[kk + 1] & XMATH_LOWER_MASK);
				mt[kk] = mt[kk + (XMATH_M - XMATH_N)] ^ (y >> 1) ^ mag01[y & 0x1];
			}
			y = (mt[XMATH_N - 1] & XMATH_UPPER_MASK) | (mt[0] & XMATH_LOWER_MASK);
			mt[XMATH_N - 1] = mt[XMATH_M - 1] ^ (y >> 1) ^ mag01[y & 0x1];

			mti = 0;
		}

		y = mt[mti++];
		y ^= XMATH_TEMPERING_SHIFT_U(y);
		y ^= XMATH_TEMPERING_SHIFT_S(y) & XMATH_TEMPERING_MASK_B;
		y ^= XMATH_TEMPERING_SHIFT_T(y) & XMATH_TEMPERING_MASK_C;
		y ^= XMATH_TEMPERING_SHIFT_L(y);

		// ET - old engine added one to the result.
		// We almost NEVER wanted to use this function
		// like this.  So, removed the +1 to return a 
		// range from 0 to n (not including n).
		return (y%n);
	}

	float xRandom::random()
	{
		float r = (float)random(MAXINT);
		float divisor = (float)MAXINT;
		return (r / divisor);
	}

	void xRandom::setRandomSeed(unsigned int n)
	{
		/* setting initial seeds to mt[N] using         */
		/* the generator Line 25 of Table 1 in          */
		/* [KNUTH 1981, The Art of Computer Programming */
		/*    Vol. 2 (2nd Ed.), pp102]                  */
		mt[0] = n & 0xffffffff;
		for (mti = 1; mti<XMATH_N; mti++)
			mt[mti] = (69069 * mt[mti - 1]) & 0xffffffff;

		seed = n;
	}

	unsigned int xRandom::getRandomSeed()
	{
		return seed;
	}

	void xRandom::randomize()
	{
		setRandomSeed((unsigned int)time(NULL));
	}

} }