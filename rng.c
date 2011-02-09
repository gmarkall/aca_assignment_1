#include <stdio.h>
#include "ss_rand.h"

int main(int argc, char **argv)
{
  int i;

  for (i=0; i<500; ++i)
  {
    printf("%f, ", drand48());
  }

  return 0;
}
