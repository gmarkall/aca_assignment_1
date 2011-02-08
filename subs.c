// subs.c

#include <stdio.h>
#include <stdlib.h>
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


void initialise(int num_particles, int random_seed, int spring_krepel, 
                int std_dev_fac, int particle_radius)
{
  numparticles = num_particles;
  rseed = random_seed;
  springkrepel = spring_krepel;
  stddevfac = std_dev_fac;
  particlerad = particle_radius;

  /* particle_array_size needs to be big enough to hold 2D *
   * coordinates for each particle.                        */
  size_t particle_array_size = 2*num_particles*sizeof(double);
  size_t radius_array_size   =   num_particles*sizeof(double);

  pparticles    = (double*)malloc(particle_array_size);
  pparticlesnew = (double*)malloc(particle_array_size);
  fparticles    = (double*)malloc(particle_array_size);
  vparticles    = (double*)malloc(particle_array_size);

  prad      = (double*)malloc(radius_array_size);
  pradstart = (double*)malloc(radius_array_size);

  if (  (pparticles == NULL)
     || (pparticlesnew == NULL)
     || (fparticles == NULL)
     || (vparticles == NULL)
     || (prad == NULL)
     || (pradstart == NULL) )
  {
    fprintf(stderr, "Allocation error.\n");
    exit(EXIT_FAILURE);
  }
}

void finalise()
{
  free(pparticles);
  free(pparticlesnew);
  free(fparticles);
  free(vparticles);
  free(prad);
  free(pradstart);
}

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
