// ss_rand.c

#include "ss_rand.h"

/* Define our own random number generator since simplescalar's gcc
 * doesn't come with it */

/* Linear congruential random number generator
 *   x[n+1] = a * x[n] mod m */
/* http://remus.rutgers.edu/~rhoads/Code/random.c no recommendation implied! */

/* return the next random number x: 0 <= x < 1*/
double drand48 ()  
{
  static unsigned int a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
  SEED = a*(SEED % q) - r*(SEED / q);
  return ((double)SEED / (double)m);
}

/* seed the generator */
void srand48 (unsigned int init) 
{
  if (init != 0) 
  {
    SEED = init;
  }
}
