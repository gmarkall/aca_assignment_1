// subs.h

#ifndef _SUBS_H
#define _SUBS_H

void initialise(int num_particles, int random_seed, double spring_krepel, 
                double std_dev_fac, double particle_radius);

void finalise();

void distribute_particles_randomly();

void output_positions(int file_index);

int particlepos(int grav_fac, int dt_fac, int min_threshold);

#endif
