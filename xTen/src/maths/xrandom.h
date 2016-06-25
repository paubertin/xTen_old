#pragma once

namespace xten { namespace xmaths {

#ifndef MAXUINT
#define MAXUINT ((unsigned int)~((unsigned int)0))
#endif // !MAXUINT
#ifndef MAXINT
#define MAXINT ((int)(MAXUINT >> 1))
#endif // !MAXUINT

//========================================================================
//
//  Original Code written at Compulsive Development
//
//========================================================================

/* Period parameters */
#define XMATH_N 624
#define XMATH_M 397
#define XMATH_MATRIX_A 0x9908b0df   /* constant vector a */
#define XMATH_UPPER_MASK 0x80000000 /* most significant w-r bits */
#define XMATH_LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define XMATH_TEMPERING_MASK_B 0x9d2c5680
#define XMATH_TEMPERING_MASK_C 0xefc60000
#define XMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define XMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define XMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define XMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

	class xRandom
	{
	private:
		unsigned int seed;
		unsigned int seed_sp;
		unsigned long mt[XMATH_N]; /* array of the state vector*/
		int mti; /* mti==N+1 means mt[N] is not initialized */

	public:
		xRandom();

		unsigned int random(unsigned int n);
		float random();
		void setRandomSeed(unsigned int n);
		unsigned int getRandomSeed();
		void randomize();
	};

	/* Example of use

	xRandom r;
	r.randomize();
	unsigned int num = r.random(100); // returns a number from 0-99, inclusive

	*/

} }