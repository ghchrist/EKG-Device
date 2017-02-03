#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    int sample_number;
    double time_stamp;
    double voltage;
} Data_Sample;

typedef struct {
    Data_Sample* samples;
    unsigned num_samples;
} Test;



void readData(void);
