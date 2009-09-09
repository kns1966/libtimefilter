/*
 * Pendule - A library for accurate time stamping
 * Copyright (c) 2009 Olivier Guilyardi <olivier samalyse com>
 * License: CeCILL-B Free Software License Agreement version 1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <jack/jack.h>
#include <jack/ringbuffer.h>
#include "pendule.h"

typedef struct TimeData {
    PenduleStats stats;
    char xrun;
} TimeData;

jack_ringbuffer_t *ringbuffer;
Pendule *pendule;
FILE *data_file = NULL;
double sample_rate;
jack_client_t *client;

static int 
process(jack_nframes_t nframes, void *arg)
{
    TimeData data;
    struct timeval tv;
    double t;

    gettimeofday(&tv, NULL);
    jack_nframes_t offset = jack_frames_since_cycle_start(client);
    t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    t -= (double) offset / sample_rate;

    pendule_update(pendule, t);
    pendule_stats(pendule, &(data.stats));
    data.xrun = 0;

    if (jack_ringbuffer_write_space(ringbuffer) >= sizeof(TimeData)) {
        jack_ringbuffer_write(ringbuffer, (char *) &data, sizeof(TimeData));
    } else {
        fprintf(stderr, "Data ringbuffer overrun\n");
    }

    return 0;
}

static int 
xrun(void *arg)
{
    TimeData data;
    data.xrun = 1;

    pendule_reset(pendule);
    if (jack_ringbuffer_write_space(ringbuffer) >= sizeof(TimeData)) {
        jack_ringbuffer_write(ringbuffer, (char *) &data, sizeof(TimeData));
    } else {
        fprintf(stderr, "Data ringbuffer overrun\n");
    }

    return 0;
}

#define STEP_DURATION 5
#define RUNTIME 60
#define BANDWIDTH 1.5

int 
main(int argc, char *argv[]) 
{
    jack_status_t status;
    client = jack_client_open("pendule", 0, &status);

    if (!client) {
        fprintf(stderr, "Can't register as JACK client\n");
        return 1;
    }

    jack_set_process_callback(client, process, (void *) client);
    jack_set_xrun_callback(client, xrun, NULL);
    sample_rate = jack_get_sample_rate(client);
    double buffer_size = jack_get_buffer_size(client);
    pendule = pendule_new(buffer_size / sample_rate, BANDWIDTH);

    ringbuffer = jack_ringbuffer_create(8192);

    if (argc > 1) {
        data_file = fopen(argv[1], "w");
        if (!data_file) {
            fprintf(stderr, "Can't open data output file for writing: %s\n", argv[1]);
            return 1;
        }
        //fprintf(data_file, "# name: data\n");
        //fprintf(data_file, "# type: matrix\n");
    }

    if (jack_activate(client) != 0) {
        fprintf(stderr, "Can't activate JACK client\n");
        return 1;
    }

    TimeData data;
    data.xrun = 0;
    uint64_t steps = 0;
    double jitter_sum = 0;
    double system_jitter_sum = 0;
    double start_time = 0;
    double ncycles = 0;

    printf("Legend (all in ms):\n");
    printf("  audiotime drift: the difference between audio and system time\n");
    printf("  filter drift: the difference between filter and system time\n");
    printf("  systime jitter: the average cycle duration error (+-), in system time\n");
    printf("  filter jitter: the average cycle duration error (+-), in filter time\n\n");

    printf("DLL bandwidth: %.2fHz\n", BANDWIDTH);

    printf("Lines are printed every %d second(s). Will run for %d minute(s).\n", STEP_DURATION, RUNTIME / 60);
    printf("+-----------------+---------------+----------------+----------------+\n");
    printf("| audiotime drift | filter drift  | systime jitter | filter jitter  |\n");
    printf("+-----------------+---------------+----------------+----------------+\n");
    while ((steps == 0) || (data.stats.system_time - start_time < RUNTIME)) {
        while (jack_ringbuffer_read_space(ringbuffer) >= sizeof(TimeData)) {
            jack_ringbuffer_read(ringbuffer, (char *) &data, sizeof(TimeData));
            if (data.xrun) {
                printf("|                               XRUN                                |\n");
            } else {
                if (ncycles == 0) {
                    start_time = data.stats.system_time;
                }
                jitter_sum += fabs(data.stats.filter_jitter);
                system_jitter_sum += fabs(data.stats.system_jitter);
                if (data.stats.system_time - start_time >= steps * STEP_DURATION) {
                    printf("| %15.6f | %13.6f | %14.6f | %14.6f |\n", 
                        data.stats.device_drift * 1000.0, 
                        data.stats.filter_drift * 1000.0, 
                        ncycles ? system_jitter_sum / ncycles * 1000.0 : 0, 
                        ncycles ? jitter_sum / ncycles * 1000.0 : 0);
                    steps++;
                }
                if (data_file) {
                    fprintf(data_file, " %.6f %.6f %.6f %.6f %.6f\n", 
                            (data.stats.system_time - start_time) * 1000.0, 
                            (data.stats.device_time - start_time) * 1000.0, 
                            (data.stats.filter_time - start_time) * 1000.0, 
                            data.stats.system_jitter * 1000.0, 
                            data.stats.filter_jitter * 1000.0);
                }
                ncycles++;
            }
        }
        usleep(250);
    }

    if (data_file) {
        fclose(data_file);
    }

    printf("+-----------------+---------------+----------------+----------------+\n\n");
    printf("average jitter correction factor: x %.2f\n", system_jitter_sum / jitter_sum);
    printf("\n");
    return 0;
}

  

