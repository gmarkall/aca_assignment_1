// subs.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
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
static double pi = 3.14159265358979; /* Pi!                          */

/* Calculate index into "2D" array. */
int p_index(int dim, int particle)
{
  int index = (dim*numparticles) + particle;
  return index;
}

/* Return the greater of two values */
double max(double u, double v)
{
  if (u > v)
  {
    return u;
  }
  else
  {
    return v;
  }
}

/* Square a number */
double sqr(double num)
{
  return pow(num, 2);
}

/* Set simulation parameters, allocate arrays */
void initialise(int num_particles, int random_seed, double spring_krepel, 
                double std_dev_fac, double particle_radius)
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

/* Free memory at the end of the simulation. */
void finalise()
{
  free(pparticles);
  free(pparticlesnew);
  free(fparticles);
  free(vparticles);
  free(prad);
  free(pradstart);
}

double normal_distribution()
{
  double output;
  double randomx, randomy, rdistheight, actualy;
  double stddev, gaussian;
  bool accept;

  stddev = stddevfac*particlerad;

  /* By default, reject the particle */
  accept = false;

  /* Get the height of the probability distribution */
  rdistheight = 1.0 / (sqrt(2.0*pi*stddev*stddev));

  /* Choose random numbers until we find an acceptable one */
  while (!accept)
  {
    randomx = drand48();
    randomy = drand48();
    randomx = (upperprad-lowerprad)*randomx + lowerprad;
    randomy = rdistheight*randomy;

    /* Find the point on the gaussian curve from randomx */
    gaussian = exp( -(sqr(randomx-particlerad)) / (2.0*sqr(stddev)) );
    actualy = (1.0 / (sqrt(2.0*pi*sqr(stddev)))) * gaussian;

    /* Test if the random number is acceptable */
    if (randomy <= actualy)
    {
      output = randomx;
      accept = true;
    }
  }

  return output;
}

void distribute_particles_randomly()
{
  int p, numparticlesold, iter;
  double theta, tabletdr;

  /* Seed the random number generator */
  srand48(rseed);

  /* Set upper and lower bounds on particle radius */
  lowerprad = max(0.0, (particlerad - 2.0*stddevfac*particlerad) );
  upperprad = particlerad + 2.0*stddevfac*particlerad;

  for (p=0; p<numparticles; ++p)
  {
    if (stddevfac>0.0)
    {
      double outputrad = normal_distribution();
      prad[p] = outputrad;
      pradstart[p] = outputrad;
    }
    else
    {
      prad[p] = particlerad;
      pradstart[p] = particlerad;
    }
  }

  /* Initialise particle array to 0 */
  for (p=0; p<numparticles; ++p)
  {
    pparticles[p_index(0,p)] = 0.0;
    pparticles[p_index(1,p)] = 0.0;
  }

  /* First particle is centred in the domain */
  pparticles[p_index(0,0)] = 0.0;
  pparticles[p_index(0,1)] = 0.0;

  /* Save the total number of particles.               */
  /* We only want to compute for the particles we have */
  numparticlesold = numparticles;
  numparticles = 1;

  /* Insert particles into the domain at the average */
  /* particle radius from the centre. (The mass of   */
  /* particles grows outwards).                      */
  for (p=1; p<numparticlesold; ++p)
  {
    theta = drand48();
    theta = theta*2.0*pi;
    tabletdr = particlerad;

    pparticles[p_index(0,p)] = cos(theta)*tabletdr;
    pparticles[p_index(1,p)] = sin(theta)*tabletdr;
    
    /* Let the system settle with the current number of */
    /* active particles.                                */
    numparticles = p;
    iter = particlepos(0.0, 0.01, 1.0);
    printf("Particle %d took %d iterations.\n", p, iter);
  }

  printf("Finished randomising particle positions.\n");
}

void output_positions(int file_index)
{
  char filename[2];
  FILE *fp;
  int p;

  filename[0] = '0'+file_index;
  filename[1] = '\0';
  fp = fopen(filename, "w");

  if (fp == NULL)
  {
    fprintf(stderr, "Error opening output file.\n");
    exit(EXIT_FAILURE);
  }

  for (p=0; p<numparticles; ++p)
  {
    double x, y, r;

    x = pparticles[p_index(0,p)];
    y = pparticles[p_index(1,p)];
    r = prad[p];

    fprintf(fp, "%15.12f\t%15.12f\t%15.12f\n", x, y, r);
  }

  fclose(fp);
}

int particlepos(int grav_fac, int dt_fac, int min_threshold)
{
  int iterations = 0;

  return iterations;
}
