// parameters.h

#ifndef _PARAMETERS_H
#define _PARAMETERS_H

// Number of particles may be varied if this isn't enough for your
// chosen architecture. You probably shouldn't be attempting to
// use less than 1000.

#define NUM_PARTICLES 1000

// Print out extra information that may help you in debugging

#define PRINT_DEBUG 0

// Don't change these from the defaults.

#define RANDOM_SEED 1234
#define SPRING_KREPEL 10.0
#define STD_DEV_FAC 0.0
#define PARTICLE_RADIUS 0.00005

#define GRAVITY_FACTOR 0.1
#define TIME_FACTOR 0.003
#define MIN_THRESHOLD 0.2

// Validation parameters. Don't change these either, unless
// instructed to.

#define MAX_RANDOM_ITERATIONS 75
#define MAX_AVG_RANDOM_ITERATIONS 45
#define MAX_GRAVITY_ITERATIONS 17500

#endif
