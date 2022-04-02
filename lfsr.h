/* Author: Dominic van der Zypen
 * Implementation of LFSR (linear feedback shift register)
 * on uint64_t using irreducible polynomial x^64 + x^61 + x^34 + x^9 + 1.
 * (On 32 bit we could use x^32 + x^22 + x^2 + x^1 + 1.)
 */

#include <stdio.h>
#include <stdint.h>

void lfsr(uint64_t* up);
//---------------------
void lfsr(uint64_t* up)
{
  uint64_t new_bit = ( (*up) ^ ((*up) >> 3) ^ ((*up) >> 30) ^ 
	             ((*up) >> 55) ) & 1u; 
	// don't have to map '+1' in polynomial
  *up = ((*up) >> 1) | (new_bit << 63); 
        // shift *up by 1 to RIGHT and insert new_bit at "empty" position
}
