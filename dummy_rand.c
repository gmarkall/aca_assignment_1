// dummy_rand.c

#include "ss_rand.h"
#include "randoms.h"

static int count;


/* return the next random number x: 0 <= x < 1*/
double drand48 ()  
{
  double output = numbers[count];
  ++count;
  return output;
}

/* seed the generator */
void srand48 (unsigned int init) 
{
  count = 0;
}
