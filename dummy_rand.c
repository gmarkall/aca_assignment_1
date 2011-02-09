// dummy_rand.c

#include "ss_rand.h"

static int count;

static double numbers[] = { 0.2, 0.4, 0.6, 0.3, 0.7, 0.5 };

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
