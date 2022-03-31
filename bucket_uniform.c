/* Author: Dominic van der Zypen
 * Goal: Uniform allocation of arbitrary unsigned ints (64 bit) to
 * a fixed number of buckets, using bit-wise operations
 */

#include <stdlib.h>
#include "lfsr.h"

//--------------------------------
// Function declarations:

unsigned char log_2(uint64_t x); 
void set_NUM_BUCKETS(unsigned int n);
void set_NUM_BITS(); 
unsigned int bucket_number(uint64_t x);
void fill_buckets(unsigned int* buckets, unsigned int iterations);
void evaluate_buckets(unsigned int* buckets, char print_all);
//--------------------------------
// Global variables:

unsigned int NUM_BUCKETS;
unsigned char NUM_BITS;

//--------------------------------
unsigned char log_2(uint64_t x)
  // ASSUME x \geq 1
  // returns smallest integer b such that 2^b = 1 << b is \geq x
  // This operation is only performed once
{
  unsigned char result = 0;
  uint64_t power_of_2 = 1;
  while (power_of_2 <= x)
  {
     power_of_2 = power_of_2 << 1;
     result++;
  }
  return (result - 1); // went one step too far in loop => '-1'
}
//--------------------------------
void set_NUM_BUCKETS(unsigned int n)
{
  NUM_BUCKETS = n;
}
//--------------------------------
void set_NUM_BITS()
{
  NUM_BITS = log_2(NUM_BUCKETS);
}
//--------------------------------
unsigned int bucket_number(uint64_t x)
  // n == number of totally available buckets, so buckets = \{0, ...,, n-1\}
  // ASSUME n < (1 << 32)
{
  uint64_t mask111 = (1 << (NUM_BITS + 1)) - 1;
  uint64_t mask011 = (1 << (NUM_BITS))   - 1; // one 1 less

  unsigned char leq = ((x & mask111) < NUM_BUCKETS); 
                                 // leq (less or equal) is 0 or 1.

  return (leq * (x & mask111)) + ((1 - leq) * ((x >> (NUM_BITS + 1)) & mask011));
    // 'x >> (NUM_BITS + 1)' : take different set of bits -> better uniformity.
    // '... & mask011' guarantees that the result is less or equal NUM_BUCKETS.
}
//--------------------------------
void fill_buckets(unsigned int* buckets, unsigned int iterations)
{
  uint64_t i = 0;
  uint64_t x = 0x98765421b16b00b5;
  unsigned int temp_bucket;
  unsigned char lfsr_iterations = (NUM_BITS << 1); // twice NUM_BITS
  unsigned char ell;

  while (i < iterations)
  {
    temp_bucket = bucket_number(x);
    *(buckets + temp_bucket) = *(buckets + temp_bucket) + 1;

    // 'turn handle' on LFSR lfsr_iteration-times!
    ell = 0;
    while (ell < lfsr_iterations)
    {
      lfsr(&x);
      ell++;
    }
    i++;
  }
}
//--------------------------------
void evaluate_buckets(unsigned int* buckets, char print_all)
{
  int i = 0;
  if (print_all)
  {
    while (i < NUM_BUCKETS)
    {
      printf("%d:%d , ", i ,  *(buckets+i));
      i++;
      if (i % 10 == 0){ printf("\n");} // formatting
    }
  }
  // TODO: get min, max, avg of buckets and print them
}
//--------------------------------
int main()
{
  int num_of_buckets = 120; // 120 is just an example of some non-power of 2
  int num_of_iterations = (1 << 20); // 1 << 20 == 2 ^ 20 == roughly 1 million

  set_NUM_BUCKETS(num_of_buckets); 
  set_NUM_BITS();

  unsigned int* buckets = (unsigned int*)malloc(NUM_BUCKETS * sizeof(unsigned int));

  // initialise buckets to 0
  int i = 0;
  while (i < NUM_BUCKETS)
  {
     *(buckets+i) = 0;
     i++;
  }
  fill_buckets(buckets, num_of_iterations);
  evaluate_buckets(buckets, 1);
  free(buckets);
  return 0;
}
