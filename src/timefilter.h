/*
 * libtimefilter - A library for accurate time stamping
 * author: Olivier Guilyardi <olivier samalyse com>
 *
 * Copyright (c) 2009, Samalyse SARL - All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * * Neither the name of Samalyse SARL nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TIMEFILTER_H
#define TIMEFILTER_H

typedef struct TimeFilter TimeFilter;

typedef struct TimeFilterStats {
    double filter_time;
    double filter_jitter;
    double filter_drift;
    double next_filter_time;

    double system_time;
    double system_jitter;

    double device_time;
    double device_drift;
} TimeFilterStats;

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
 * timefilter_update()
 */
TimeFilter * timefilter_new(double period, double bandwidth); 

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
void      timefilter_update(TimeFilter *self, double system_time);

/**
 * Retrieve the filtered time
 *
 * The returned value represents the filtered time of the beginning of the 
 * current cycle as updated by the last call to timefilter_update()
 *
 * Warning: you must call timefilter_update() before this, otherwise the result
 * is undetermined.
 */
double    timefilter_gettime(TimeFilter *);

/**
 * Reset the filter
 *
 * This function should mainly be called in case of XRUN. 
 *
 * Warning: after calling this, the filter is in an undetermined state until
 * the next call to timefilter_update()
 */
void      timefilter_reset(TimeFilter *); 

/**
 * Retrieve various statistics
 *
 * Warning: you must call timefilter_update() before this, otherwise the results
 * are undetermined.
 */
void      timefilter_stats(TimeFilter *, TimeFilterStats *stats);

/**
 * Free all resources associated with the filter
 */
void      timefilter_destroy(TimeFilter *);

#endif





