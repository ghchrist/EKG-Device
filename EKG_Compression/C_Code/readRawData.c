#include "readRawData.h"

Test test;

int main(void){
   //read in data and store into structures for further use
   readData();

   return 0;
}


//this function reads all sample EKG data points from input file and stores these
//data points into structs
void readData(void)
{
    char buffer[50];
    char* token;
    unsigned num_samples = 0;

    //FILE *fp = fopen("../ECG_MIT_Data/a_fib/04126_ecg2_10.csv", "r");
    //FILE *fp = fopen("../ECG_MIT_Data/a_fib/07879_ecg2_10.csv", "r");
    FILE *fp = fopen("../ECG_MIT_Data/normal_sinus/16773_10_ecg1.csv", "r");
    //FILE *fp = fopen("../ECG_MIT_Data/normal_sinus/18184_ecg2_60.csv", "r");

    //make sure we can open the file
    if(fp == NULL) {
        printf("cannot open file\n");
        exit(1);
    }

    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    //determines how many sample data points there are in file
    //used to allocated memory for stuctures
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
      //tokenizes line and store the time_stamp into token
      token = strtok(buffer, ",");

      if(atof(token) <= 6.0){
         num_samples++;
      }
    }

    //alloacates memory for samples pointer struct based off how many sample data points there are
    test.num_samples = num_samples;
    test.samples = malloc(sizeof(Data_Sample) * num_samples);
    num_samples = 0;

    rewind(fp);

    //skips first 2 lines in sample data file
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    //reads in data one line at a time
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
         //tokenizes line and stores the time_stamp into token
         token = strtok(buffer, ",");

         if(atof(token) <= 6.0){
            test.samples[num_samples].sample_number = num_samples;  //loads data point sample # into Data_Sample struct
            test.samples[num_samples].time_stamp = atof(token);     //loads time stamp of data point into Data_Sample struct

            //tokenizes line and stores the voltage into token
            token = strtok(NULL, ",");
            test.samples[num_samples].voltage = atof(token);        //loads data point voltage into Data_Sample struct

            printf("%i -> %f, %f\n",test.samples[num_samples].sample_number,test.samples[num_samples].time_stamp,test.samples[num_samples].voltage);

            num_samples++;
         }
    }

    printf("Number of Samples: %i\n", num_samples);

    fclose(fp);
}
