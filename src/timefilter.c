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

/*
 * This code is heavily based on Fons Adriaensen's essay "Using a DLL to
 * filter time", available at: http://www.kokkinizita.net/papers/usingdll.pdf
 */

#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include "timefilter.h"

struct TimeFilter {

    // Delay Locked Loop
    double tper;
    double b;
    double c;
    double e2;
    double t0;
    double t1;

    // Stats
    double filter_time;
    double system_time;
    double device_time;
    double filter_period_error;
    double system_period_error;
    double ncycles;
};

TimeFilter *
timefilter_new(double period, double bandwidth) 
{
    double o;
    TimeFilter *self = calloc(1, sizeof(TimeFilter));
    self->tper       = period;
    o                = 2 * M_PI * bandwidth * self->tper;
    self->b          = sqrt(2 * o);
    self->c          = o * o;
    self->t0         = 0;
    return self;
}

void
timefilter_destroy(TimeFilter *self)
{
    free(self);
}

void
timefilter_reset(TimeFilter *self) 
{
    self->t0 = 0;
}

void
timefilter_update(TimeFilter *self, double system_time)
{
    double e;

    if (self->t0 == 0) {
        // init loop
        self->e2 = self->tper;
        self->t0 = system_time;
        self->t1 = self->t0 + self->e2;

        // init stats
        self->device_time         = system_time;
        self->system_period_error = self->filter_period_error = 0;
        self->ncycles             = 0;
    } else {
        // calculate loop error
        e = system_time - self->t1;

        // update loop
        self->t0  = self->t1;
        self->t1 += self->b * e + self->e2;
        self->e2 += self->c * e;

        // update stats
        self->filter_period_error = self->t0 - self->filter_time - self->tper;
        self->system_period_error = system_time - self->system_time - self->tper;
        self->device_time        += self->tper;
        self->ncycles++;
    }

    self->system_time = system_time;
    self->filter_time = self->t0;
}

double
timefilter_gettime(TimeFilter *self) 
{
    return self->filter_time;
}

void
timefilter_stats(TimeFilter *self, TimeFilterStats *stats) 
{
    double device_rate_error;
    stats->filter_time      = self->filter_time;
    stats->next_filter_time = self->t1;
    stats->system_time      = self->system_time;
    stats->device_time      = self->device_time;
    stats->filter_drift     = self->filter_time - self->system_time;
    stats->device_drift     = stats->device_time - self->system_time;
    device_rate_error       = self->ncycles ? stats->device_drift / self->ncycles : 0;
    stats->filter_jitter    = self->filter_period_error - device_rate_error;
    stats->system_jitter    = self->system_period_error - device_rate_error;
}
