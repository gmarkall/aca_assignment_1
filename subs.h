// subs.h

#ifndef _SUBS_H
#define _SUBS_H

void initialise(int num_particles, int random_seed, double spring_krepel, 
                double std_dev_fac, double particle_radius);

void finalise();

void distribute_particles_randomly();

void output_positions(int file_index);

int particlepos(double grav_fac, double dt_fac, double min_threshold);

#endif
