//=============================================
// DEM Settling code
//
// Original by James Kimber
//
// Ported to C/Simplescalar by Graham Markall
//
//=============================================

#include <subs.h>

// Simulation parameters. Don't change these from the defaults.

#define NUM_PARTICLES 1000
#define RANDOM_SEED 1234
#define SPRING_KREPEL 10.0
#define STD_DEV_FAC 0.0
#define PARTICLE_RADIUS 5.0E-05

#define GRAVITY_FACTOR 0.1
#define TIME_FACTOR 0.003
#define MIN_THRESHOLD 0.2

int main(int argc, char **argv)
{
  int iterations;

  distribute_particles_randomly();
  output_positions(0);

  iterations = particlepos(GRAVITY_FACTOR, TIME_FACTOR, MIN_THRESHOLD);

  printf("Converged in %d iterations", iterations);
  output_positions(1);
}
