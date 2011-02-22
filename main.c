//=============================================
// DEM Settling code
//
// Original by James Kimber
//
// Ported to C/Simplescalar by Graham Markall
//
//=============================================

#include <stdio.h>
#include "subs.h"
#include "parameters.h"

int main(int argc, char **argv)
{
  int iterations;

  initialise(NUM_PARTICLES, RANDOM_SEED, SPRING_KREPEL, STD_DEV_FAC, PARTICLE_RADIUS);

  distribute_particles_randomly();
  output_positions(0);

  print_metrics(1.0);

  iterations = particlepos(GRAVITY_FACTOR, TIME_FACTOR, MIN_THRESHOLD);
  
  printf("With gravity, converged in %d iterations\n", iterations);
  output_positions(1);

  print_metrics(MIN_THRESHOLD);

  finalise();

  return 0;
}
