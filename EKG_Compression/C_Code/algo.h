#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <pthread.h>


#define MAX_R_PEAKS 200
#define MAX_SAMPLES 10000

typedef struct {
    unsigned sample_number;
    double time_stamp;
    double voltage;
} Data_Sample;

typedef struct {
    unsigned start;
    unsigned end;
} BOUNDARY;

typedef struct {
    Data_Sample* samples;
    unsigned num_samples;
} Test;


void average_boundaries(void);

unsigned find_p_wave(BOUNDARY boundary);

void set_boundaries(unsigned);

unsigned find_r_peaks(void);

unsigned find_r_peaks_inverted(void);

void populate_data(void);

unsigned max(void);

unsigned min(void);

unsigned min_in_boundary(BOUNDARY boundary);

void quick_sort(int, int);

int partition(int, int);

double findStandardDeviation(unsigned, double[]);
