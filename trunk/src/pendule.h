/*
 * Pendule - A library for accurate time stamping
 * Copyright (c) 2009 Olivier Guilyardi <olivier samalyse com>
 * License: CeCILL-B Free Software License Agreement version 1.0
 */

#ifndef PENDULE_H
#define PENDULE_H

typedef struct Pendule Pendule;

typedef struct PenduleStats {
    double filter_time;
    double filter_jitter;
    double filter_drift;
    double next_filter_time;

    double system_time;
    double system_jitter;

    double device_time;
    double device_drift;
} PenduleStats;

/**
 * Create a new DLL time filter
 *
 * Period must be the device cycle duration in seconds. For example, at 44.1Hz
 * and a buffer size of 512 frames, period = 512 / 44100. The filter only works
 * if the cycle duration is fixed.
 *
 * The bandwidth is up to you to choose. Smaller values will filter out more
 * of the jitter, but also take a longer time for the loop to settle. A good 
 * starting point is something between 0.3 and 3 Hz.
 *
 * Warning: the filter is not fully initialized until the first call to 
 * pendule_update()
 */
Pendule * pendule_new(double period, double bandwidth); 

/**
 * Update the filter
 * 
 * This function must be called in real time, at each process cycle.  
 *
 * system_time, in seconds, should be as closest as possible to the moment
 * the device interrupt occured.
 *
 * The easiest way to achieve this is to retrieve the system time as early as 
 * possible in each cycle. 
 *
 * Note: in JACK, you can compensate the delay between the interrupt and the 
 * moment your process callback is called using jack_frames_since_cycle_start().
 */
void      pendule_update(Pendule *self, double system_time);

/**
 * Retrieve the filtered time
 *
 * The returned value represents the filtered time of the beginning of the 
 * current cycle as updated by the last call to pendule_update()
 *
 * Warning: you must call pendule_update() before this, otherwise the result
 * is undetermined.
 */
double    pendule_gettime(Pendule *);

/**
 * Reset the filter
 *
 * This function should mainly be called in case of XRUN. 
 *
 * Warning: after calling this, the filter is in an undetermined state until
 * the next call to pendule_update()
 */
void      pendule_reset(Pendule *); 

/**
 * Retrieve various statistics
 *
 * Warning: you must call pendule_update() before this, otherwise the results
 * are undetermined.
 */
void      pendule_stats(Pendule *, PenduleStats *stats);

/**
 * Free all resources associated with the filter
 */
void      pendule_destroy(Pendule *);

#endif





