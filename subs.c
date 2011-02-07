// subs.c

#include "subs.h"

static double* pparticles;           /* Particle x, y coordinates    */
static double* pparticlesnew;        /* Updated particle coordinates */
static double* fparticles;           /* Interparticle forces         */
static double* vparticles;           /* Particle velocities          */
static double* prad;                 /* Particle radii               */
static double* pradstart;            /* Initial particle radii       */

static int numparticles;             /* Number of particles          */
static int rseed;                    /* Random seed                  */

static double springkrepel;          /* Global repulsive forces      */
static double stddevfac;             /* Std deviation factor         */
static double lowerprad;             /* Particle radius lower bound  */
static double upperprad;             /* Particle radius upper bound  */
static double particlerad;           /* Mean particle radius         */

void distribute_particles_randomly()
{

}

void output_positions(int file_index)
{

}

int particlepos(int grav_fac, int dt_fac, int min_threshold)
{
  int iterations = 0;

  return iterations;
}
