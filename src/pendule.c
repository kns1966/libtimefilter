/*
 * Pendule - A library for accurate time stamping
 * Copyright (c) 2009 Olivier Guilyardi <olivier samalyse com>
 * License: CeCILL-B Free Software License Agreement version 1.0
 *
 * This code is heavily based on Fons Adriaensen's essay "Using a DLL to
 * filter time", available at: http://www.kokkinizita.net/papers/usingdll.pdf
 */

#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include "pendule.h"

struct Pendule {

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

Pendule *
pendule_new(double period, double bandwidth) 
{
    double o;
    Pendule *self = calloc(1, sizeof(Pendule));
    self->tper = period;
    o = 2 * M_PI * bandwidth * self->tper;
    self->b = sqrt(2 * o);
    self->c = o * o;
    self->t0 = 0;
    return self;
}

void
pendule_destroy(Pendule *self)
{
    free(self);
}

void
pendule_reset(Pendule *self) 
{
    self->t0 = 0;
}

void
pendule_update(Pendule *self, double system_time)
{
    double e;

    if (self->t0 == 0) {
        // init loop
        self->e2 = self->tper;
        self->t0 = system_time;
        self->t1 = self->t0 + self->e2;

        // init stats
        self->device_time = system_time;
        self->system_period_error = self->filter_period_error = 0;
        self->ncycles = 0;
    } else {
        // calculate loop error
        e = system_time - self->t1;

        // update loop
        self->t0 = self->t1;
        self->t1 += self->b * e + self->e2;
        self->e2 += self->c * e;

        // update stats
        self->filter_period_error = self->t0 - self->filter_time - self->tper;
        self->system_period_error = system_time - self->system_time - self->tper;
        self->device_time += self->tper;
        self->ncycles++;
    }

    self->system_time = system_time;
    self->filter_time = self->t0;
}

double
pendule_gettime(Pendule *self) 
{
    return self->filter_time;
}

void
pendule_stats(Pendule *self, PenduleStats *stats) 
{
    double device_rate_error;
    stats->filter_time = self->filter_time;
    stats->next_filter_time = self->t1;
    stats->system_time = self->system_time;
    stats->device_time = self->device_time;
    stats->filter_drift = self->filter_time - self->system_time;
    stats->device_drift = stats->device_time - self->system_time;
    device_rate_error = self->ncycles ? stats->device_drift / self->ncycles : 0;
    stats->filter_jitter = self->filter_period_error - device_rate_error;
    stats->system_jitter = self->system_period_error - device_rate_error;
}
