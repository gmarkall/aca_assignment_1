/* subs.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ss_bool.h"
#include "ss_rand.h"
#include <string.h>
#include "subs.h"

/* How long do we iterate for before deciding to escape? */
#define MAX_ITER 50000
/* How many dips below threshold force before we exit the iteration loop? */
#define MAX_DIPS_BELOW_THRESHOLD 2

static double* pparticles;           /* Particle x, y coordinates    */
static double* pparticlesnew;        /* Updated particle coordinates */
static double* fparticles;           /* Interparticle forces         */
static double* vparticles;           /* Particle velocities          */
static double* prad;                 /* Particle radii               */
static double* pradstart;            /* Initial particle radii       */

static int numparticles;             /* Number of particles          */
static int numparticlesold;          /* Number of particles (again)  */
static int rseed;                    /* Random seed                  */

static double springkrepel;          /* Global repulsive forces      */
static double stddevfac;             /* Std deviation factor         */
static double lowerprad;             /* Particle radius lower bound  */
static double upperprad;             /* Particle radius upper bound  */
static double particlerad;           /* Mean particle radius         */
static double damping;               /* Damping factor               */
static double pi = 3.14159265358979; /* Pi!                          */

/* Calculate index into "2D" array. */
int p_index(int dim, int particle)
{
  int index = (dim*numparticlesold) + particle;
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

/* Cube a number */
double cube(double num)
{
  return pow(num, 3);
}

/* Return the smallest value in the prad array, excluding entries */
/* that are zero.                                                 */
double minval_prad()
{
  /* This initial value *should* be greater than any in prad */
  double output = 10.0;

  int i;

  for (i=0; i<numparticles; ++i)
  {
    double candidate = prad[i];

    if (candidate > 0.0)
    {
      if (candidate < output)
      {
        output = candidate;
      }
    }
  }

  return output;
}

/* Set simulation parameters, allocate arrays */
void initialise(int num_particles, int random_seed, double spring_krepel, 
                double std_dev_fac, double particle_radius)
{
  unsigned int particle_array_size;
  unsigned int radius_array_size;
  
  numparticles = num_particles;
  numparticlesold = num_particles;
  rseed = random_seed;
  springkrepel = spring_krepel;
  stddevfac = std_dev_fac;
  particlerad = particle_radius;

  /* particle_array_size needs to be big enough to hold 2D */
  /* coordinates for each particle.                        */
  particle_array_size = 2*num_particles*sizeof(double);
  radius_array_size   =   num_particles*sizeof(double);

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
  double randomx, randomy, rdistheight, actualy;
  double stddev, gaussian;

  stddev = stddevfac*particlerad;

  /* Get the height of the probability distribution */
  rdistheight = 1.0 / (sqrt(2.0*pi*stddev*stddev));

  /* Choose random numbers until we find an acceptable one */
  while (true)
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
      return randomx;
    }
  }
}

void distribute_particles_randomly()
{
  int p, iter;
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
  pparticles[p_index(1,0)] = 0.0;

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
    ++numparticles;
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

    fprintf(fp,"%17.16f\t%17.16f\t%17.16f\n",x,y,r);
  }

  fclose(fp);
}

int particlepos(double grav_fac, double dt_fac, double min_threshold)
{
  /* Number of update iterations */
  int iterations = 0;
  /* Particle index numbers */
  int p, n;
  /* Number of iterations where the force has dropped below threshold */
  int dipsbelowthreshold = 0;
  /* Distance between particles */
  double dxp, dyp;
  /* Force between particles */
  double fxp, fyp;
  /* Average force, max allowable force, overlap magnitude */
  double avforce, fmax, vecscale;
  /* Particle radius */
  double prn, prp;
  /* Particle x and y coords */
  double nx, ny, px, py;
  /* Change in particle x and y */
  double deltax, deltay;
  /* Distance of current particle from origin */
  double dist;
  /* Min force from smallest allowable overlap */
  double fmin;
  /* Gravity force, magnitude of force, sum of radii */
  double fgrav, fmag, rsum;
  /* Maximum movement a particle can do in a timestep */
  double pmovemax, pmovemin;
  /* Particle mass */
  double mass;
  /* Internal time step */
  double dtint;

  /* The most we want to move is 1/20th the smallest */
  /* radius in the system.                           */
  pmovemax = minval_prad() / 20.0;
  /* The minimum we want a particle to move is       */
  /* is 1/1000th of the max.                         */
  pmovemin = pmovemax/1000.0;
  /* Min and max forces as a result of some small    */
  /* overlap.                                        */
  fmax = springkrepel*pmovemax;
  fmin = springkrepel*min_threshold*minval_prad();

  /* Damping  and timestep                           */
  /* are from simple harmonic motion. These affect   */
  /* stability and convergence speed.                */
  damping = 20.0*sqrt( cube( minval_prad() / particlerad ) * springkrepel);
  dtint =  dt_fac*sqrt( cube( minval_prad() / particlerad ) / springkrepel);
  fgrav = 50.0 * fmax * grav_fac;
  /* This large value to prevent the while loop terminating immediately */
  avforce = 100.0;
  
  /* Initialise velocities to zero */
  for (p=0; p<numparticles; ++p)
  {
    vparticles[p_index(0,p)] = 0.0;
    vparticles[p_index(1,p)] = 0.0;
  }

  /* Iterate until force has been below threshold for 3 iterations */
  while (dipsbelowthreshold <= MAX_DIPS_BELOW_THRESHOLD)
  {
    if (avforce < fmin)
    {
      ++dipsbelowthreshold;
    }

    /* Initialise forces between particles to zero */
    for (p=0; p<numparticles; ++p)
    {
      fparticles[p_index(0,p)] = 0.0;
      fparticles[p_index(1,p)] = 0.0;
    }

    /* Compute the gravity vector */
    for (p=0; p<numparticles; ++p)
    {
      px = pparticles[p_index(0,p)];
      py = pparticles[p_index(1,p)];
      
      dist = sqrt( sqr(px) + sqr(py) );
      
      /* At the origin we need to leave the gravity vector as zero */
      if (dist != 0.0)
      {
        dxp = px / dist;
	dyp = py / dist;
	fparticles[p_index(0,p)] = -dxp*fgrav;
	fparticles[p_index(1,p)] = -dyp*fgrav;
      }
    }
    
    /* Resolve the forces between the p-th and n-th particles */
    for (n=0; n < (numparticles-1); ++n)
    {
      nx = pparticles[p_index(0,n)];
      ny = pparticles[p_index(1,n)];
      prn = prad[n];
      
      for (p=(n+1); p < (numparticles); ++p)
      {
        dxp = pparticles[p_index(0,p)] - nx;
        dyp = pparticles[p_index(1,p)] - ny;
	prp = prad[p];
	rsum = prp + prn;
	dist = sqr(dxp) + sqr(dyp);

	/* If particles overlap, compute the force between them */
	if (dist < sqr(rsum))
	{
          dist = sqrt(dist);
	  vecscale = rsum - dist;
	  fmag = springkrepel * (vecscale / rsum);
	  fxp = (fmag * dxp) / dist;
	  fyp = (fmag * dyp) / dist;
	  
	  fparticles[p_index(0,p)] += fxp;
	  fparticles[p_index(1,p)] += fyp;
	  fparticles[p_index(0,n)] -= fxp;
	  fparticles[p_index(1,n)] -= fyp;
	}
      }
    }

    /* Compute average force between particles */
    avforce = 0.0;
    for (p=0; p<numparticles; ++p)
    {
      avforce += sqrt( sqr(fparticles[p_index(0,p)]) 
                     + sqr(fparticles[p_index(1,p)]) );
    }
    avforce = avforce / numparticles;

    /* Compute force on particles after damping */
    for (p=0; p<numparticles; ++p)
    {
      fparticles[p_index(0,p)] -= damping * vparticles[p_index(0,p)];
      fparticles[p_index(1,p)] -= damping * vparticles[p_index(1,p)];
    }

    /* The remainder of the code in this block computes  */
    /* the new particles positions by integrating F = ma */
    
    /* Update velocities based on forces, compute new positions */
    for (p=0; p<numparticles; ++p)
    {
      mass = cube( prad[p] / pradstart[p] );
      vparticles[p_index(0,p)] += (fparticles[p_index(0,p)] / mass) * dtint;
      vparticles[p_index(1,p)] += (fparticles[p_index(1,p)] / mass) * dtint;

      deltax = vparticles[p_index(0,p)] *dtint;
      deltay = vparticles[p_index(1,p)] *dtint;

      pparticlesnew[p_index(0,p)] = pparticles[p_index(0,p)] + deltax;
      pparticlesnew[p_index(1,p)] = pparticles[p_index(1,p)] + deltay;
    }
    
    /* Update positions */
    for (p=0; p<numparticles; ++p)
    {
      pparticles[p_index(0,p)] = pparticlesnew[p_index(0,p)];
      pparticles[p_index(1,p)] = pparticlesnew[p_index(1,p)];
    }

    ++iterations;

    if (iterations > MAX_ITER)
    {
      fprintf(stderr, "MAX_ITER Exceeded. Computational error?\n");
      dipsbelowthreshold = MAX_DIPS_BELOW_THRESHOLD+1;
    }
  }

  return iterations;
}
