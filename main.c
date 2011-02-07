//=============================================
// DEM Settling code
//
// Original by James Kimber
//
// Ported to C/Simplescalar by Graham Markall
//
//=============================================

// Simulation parameters. Don't change these from the defaults.

#define NUM_PARTICLES 1000
#define RANDOM_SEED 1234
#define SPRING_KREPEL 10.0
#define STD_DEV_FAC 0.0
#define PARTICLE_RADIUS 5.0E-05

int main(int argc, char **argv)
{
  int iterations;

  distribute_particles_randomly();
  output_positions(0);

  iterations = particlepos(0.1, 0.003, 0.2); // What are these magic numbers?

  printf("With gravity, converged in %d iterations", iterations);
  output_positions(1);
}
