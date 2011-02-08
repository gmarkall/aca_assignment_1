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

// Simulation parameters. Don't change these from the defaults.

#define NUM_PARTICLES 1000
#define RANDOM_SEED 1234
#define SPRING_KREPEL 10.0
#define STD_DEV_FAC 0.0
#define PARTICLE_RADIUS 0.00005

#define GRAVITY_FACTOR 0.1
#define TIME_FACTOR 0.003
#define MIN_THRESHOLD 0.2

int main(int argc, char **argv)
{
  int iterations;

  initialise(NUM_PARTICLES, RANDOM_SEED, SPRING_KREPEL, STD_DEV_FAC, PARTICLE_RADIUS);

  distribute_particles_randomly();
  output_positions(0);

  iterations = particlepos(GRAVITY_FACTOR, TIME_FACTOR, MIN_THRESHOLD);

  printf("Converged in %d iterations", iterations);
  output_positions(1);

  finalise();

  return 0;
}
