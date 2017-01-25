#include "algo.h"

Data_Sample r_peaks[MAX_R_PEAKS];
Test test;
BOUNDARY boundaries[MAX_R_PEAKS * 2];
unsigned num_boundaries = 0;

int main(void)
{
    unsigned num_r_peaks;
    populate_data();

    //fab() returns the absolute value of a floating point number
    if(fabs(test.samples[min()].voltage) > test.samples[max()].voltage) {
        printf("***min***\n");
        num_r_peaks = find_r_peaks_inverted();
    }
    else {
        printf("***max***\n");
        num_r_peaks = find_r_peaks();
    }

    quick_sort(0, num_r_peaks);

    //printf("num_r_peaks = %d \n", num_r_peaks);

    double rr_intervals[MAX_R_PEAKS];

    for(int i = 0; i < num_r_peaks; i++) {
        rr_intervals[i] =  r_peaks[i+1].time_stamp - r_peaks[i].time_stamp;
        //printf("RR Interval = %lf \n", rr_intervals[i]);
    }


    set_boundaries(num_r_peaks);

    average_boundaries();

    //printf("index of p_wave = %d \n", find_p_wave(boundaries[1]));
    //printf("time_stamp of p= %lf\n", test.samples[find_p_wave(boundaries[1])].time_stamp);

    //printf("standard deviation = %lfms \n", 1000*findStandardDeviation(num_r_peaks, rr_intervals));

    free(test.samples);
    return 0;
}

void average_boundaries(void)
{
    unsigned i, j;
    unsigned min_qrs_samples = boundaries[1].end - boundaries[1].start;
    //unsigned saved_index = 1;


    for(i = 1; i < num_boundaries; i++) {
        //printf("num samples in boundary %u: %u\n", i, boundaries[i].end - boundaries[i].start);
        if(boundaries[i].end - boundaries[i].start < min_qrs_samples) {
            min_qrs_samples = boundaries[i].end - boundaries[i].start;
        }
    }

    //double avg_qrs[num_boundaries-1][min_qrs_samples];
    double avg[min_qrs_samples];
    for(i = 0; i < min_qrs_samples; i++) {
        double sum = 0;
        for(j = 1; j < num_boundaries; j++) {
            //count++;
            //printf("count = %d\n", count);
            sum += test.samples[boundaries[j].start + i].voltage;
        }
        avg[i] = sum / (double)min_qrs_samples;
        //printf("avg[i] = %lf\n", avg[i]);
    }

    FILE *output_file = fopen("arduino_output_results.txt", "w");

    for(i = 0; i < min_qrs_samples; i++) {
        //if(i % 10 == 0)
            //fprintf(output_file, "\n");

        fprintf(output_file, "%lf\n", avg[i]);
    }

    fclose(output_file);
    //FILE *fp = fopen("")

}



void set_boundaries(unsigned num_r_peaks)
{
    unsigned i;
    printf("num_r_peaks = %d\n", num_r_peaks);
    for(i = 2; i < num_r_peaks-1; i++) {
        boundaries[i-1].start = r_peaks[i].sample_number - ((r_peaks[i].sample_number - r_peaks[i-1].sample_number) / 2);
        boundaries[i-1].end = r_peaks[i].sample_number + ((r_peaks[i+1].sample_number - r_peaks[i].sample_number) / 2) + 10;
        num_boundaries++;
    }

    /*for(i = 1; i < num_r_peaks - 2; i++) {
        printf("i = %u     ", i);
        printf("start = %u  ", boundaries[i].start);
        printf("end = %u\n", boundaries[i].end);
    }*/

    printf("num_boundaries = %d\n", num_boundaries);

    /*for(i = 1; i < num_boundaries; i++) {
        printf("i = %u     ", i);
        printf("start = %u  ", boundaries[i].start);
        printf("end = %u\n", boundaries[i].end);
    }*/

}

void populate_data(void)
{
    char buffer[50];
    char* token;
    unsigned num_samples = 0;

    //FILE *fp = fopen("GOOD OUTPUT.txt", "r");

    //FILE *fp = fopen("../ECG_MIT_Data/a_fib/04126_ecg2_10.csv", "r");
    FILE *fp = fopen("../ECG_MIT_Data/a_fib/07879_ecg2_10.csv", "r");
    //FILE *fp = fopen("../ECG_MIT_Data/normal_sinus/16773_10_ecg1.csv", "r");
    //FILE *fp = fopen("../ECG_MIT_Data/normal_sinus/18184_ecg2_60.csv", "r");

    if(fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    //determines how many sample data points there are in file
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        num_samples++;
    }

    printf("*****num_samples = %u\n", num_samples);

    //creates test struct
    //loads in the number of sample data points
    //alloacates memory for samples pointer struct based off how many sample data points there are
    test.num_samples = num_samples;
    test.samples = malloc(sizeof(Data_Sample) * num_samples);
    num_samples = 0;

    rewind(fp);

    //skips first 2 lines in sample data file
    //EX:
    //'Elapsed time','ECG2'
    //'seconds','mV'
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    //reads every line of data points from input file containing ECG data
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        //Tokenizes data point pairs that are sepparated by a ","
        token = strtok(buffer, ",");
        test.samples[num_samples].sample_number = num_samples;  //loads data point sample # into Data_Sample struct
        test.samples[num_samples].time_stamp = atof(token);     //loads time stamp of data point into Data_Sample struct
        token = strtok(NULL, ",");
        test.samples[num_samples].voltage = atof(token);        //loads data point voltage into Data_Sample struct

        printf("%f", test.samples[num_samples].time_stamp);
        printf(",");
        printf("%f\n", test.samples[num_samples].voltage);

        num_samples++;
    }

    fclose(fp);
}

unsigned find_r_peaks(void)
{
    //populate_data();

    unsigned i = 0;
    unsigned saved_index;
    unsigned r_peaks_index = 0;

    unsigned global_max_index = max();

    double global_max_voltage = test.samples[global_max_index].voltage;

    while(1) {
        saved_index = max();

        if(test.samples[saved_index].voltage > global_max_voltage/2) {
            r_peaks[r_peaks_index].voltage = test.samples[saved_index].voltage;
            r_peaks[r_peaks_index].time_stamp = test.samples[saved_index].time_stamp;
            r_peaks[r_peaks_index].sample_number = test.samples[saved_index].sample_number;

            r_peaks_index++;

            for(i = saved_index + 8; i >= saved_index; i--) {
                test.samples[i].voltage = 0;
            }
            for(i = saved_index - 8; i <= saved_index; i++) {
                test.samples[i].voltage = 0;
            }
        }
        else {
            break;
        }
    }

    free(test.samples);

    populate_data();

    /*for(int i = 0; i < r_peaks_index; i++) {
        printf("sample_number = %u\ntime_stamp = %lf\nvoltage = %lf\n",
        test.samples[r_peaks[i].sample_number].sample_number,
        test.samples[r_peaks[i].sample_number].time_stamp,
        test.samples[r_peaks[i].sample_number].voltage);

        printf("\n");
    }*/

    //printf("num r peaks = %u\n", r_peaks_index);

    return r_peaks_index; //returns the number of R peaksin ECG test
}

unsigned find_r_peaks_inverted(void)
{
    //populate_data();

    unsigned i = 0;
    unsigned saved_index;
    unsigned r_peaks_index = 0;

    unsigned global_min_index = min();

    double global_min_voltage = test.samples[global_min_index].voltage;

    while(1) {
        saved_index = min();

        if(test.samples[saved_index].voltage < global_min_voltage/2) {
            r_peaks[r_peaks_index].voltage = test.samples[saved_index].voltage;
            r_peaks[r_peaks_index].time_stamp = test.samples[saved_index].time_stamp;
            r_peaks[r_peaks_index].sample_number = test.samples[saved_index].sample_number;

            r_peaks_index++;

            for(i = saved_index + 8; i >= saved_index; i--) {
                test.samples[i].voltage = 0;
            }
            for(i = saved_index - 8; i <= saved_index; i++) {
                test.samples[i].voltage = 0;
            }
        }
        else {
            break;
        }
    }

    free(test.samples);

    populate_data();

    /*for(int i = 0; i < r_peaks_index; i++) {
        printf("sample_number = %u\ntime_stamp = %lf\nvoltage = %lf\n",
        test.samples[r_peaks[i].sample_number].sample_number,
        test.samples[r_peaks[i].sample_number].time_stamp,
        test.samples[r_peaks[i].sample_number].voltage);

        printf("\n");
    }*/

    //printf("num r peaks = %u\n", r_peaks_index);

    return r_peaks_index; //returns the number of R peaksin ECG test
}




double findStandardDeviation(unsigned num_r_peaks, double rr_intervals[])
{
    unsigned i = 0;
    double avg = 0;
    double std_deviation = 0.0;
    double variance = 0.0;

    for(i = 0; i < num_r_peaks; i++) {
        avg += rr_intervals[i];
    }
    avg = avg / num_r_peaks;

    printf("average = %lfms \n", 1000*avg);

    for(i = 0; i < num_r_peaks; i++) {
        variance = (avg - rr_intervals[i]) * (avg - rr_intervals[i]);
    }

    std_deviation = sqrt(variance);

    return std_deviation;
}

//finds the max voltage value for a patients data samples
//returns the index of the max voltage value
unsigned max(void)
{
    double max = test.samples[0].voltage;
    unsigned i = 0;
    unsigned saved_index;

    //printf("test.num_samples = %d\n", test.num_samples);

    for(i = 0; i < test.num_samples; i++) {
        if(test.samples[i].voltage > max) {
            //printf("here\n");
            max = test.samples[i].voltage;
            saved_index = i;
        }
    }

    return saved_index;
}

//finds the min voltage value for a patients data samples
//returns the index of the min voltage value
unsigned min(void)
{
    double min = test.samples[0].voltage;
    unsigned i = 0;
    unsigned saved_index;

    //printf("test.num_samples = %d\n", test.num_samples);

    for(i = 0; i < test.num_samples; i++) {
        if(test.samples[i].voltage < min) {
            //printf("here\n");
            min = test.samples[i].voltage;
            saved_index = i;
        }
    }

    return saved_index;
}


void quick_sort(int l, int r)
{
   int j;

   if( l < r )
   {
   	// divide and conquer
       j = partition(l, r);
       quick_sort(l, j-1);
       quick_sort(j+1, r);
   }
}

int partition(int l, int r)
{
   double pivot;
   int i, j;
   Data_Sample t;
   pivot = r_peaks[l].time_stamp;
   i = l;
   j = r+1;

   while(1) {
       do
          ++i;
       while( r_peaks[i].time_stamp <= pivot && i <= r );
   	   do
          --j;
       while( r_peaks[j].time_stamp > pivot );
   	   if( i >= j )
          break;
       t = r_peaks[i];
       r_peaks[i] = r_peaks[j];
       r_peaks[j] = t;
   }

   t = r_peaks[l];
   r_peaks[l] = r_peaks[j];
   r_peaks[j] = t;

   return j;
}
