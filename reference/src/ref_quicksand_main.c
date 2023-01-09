#include <ref_quicksand.h>
#include <ANSI-color-codes.h>

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/time.h>




double check_ref_qs(uint8_t *input , int input_len , uint8_t *output,int number_of_tests){
    
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    for(int i =0; i< number_of_tests ; i++){
        ref_qs_init();
        ref_qs_update(input , input_len);
        ref_qs_digest(output);
        ref_qs_cleanup();
    }
    
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program. 
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    return time_taken/number_of_tests;
    
}

void print_hex(char *str,uint8_t *output , int len){
    printf(BWHT"%s"reset, str);
    printf(CYN);
    for(int i =0; i < len ; i++){
        printf("%02X", output[i]);
    }
    printf(reset"\n");

}



int main(){
    int input_len = 1000000;
    int output_len = 128;
    int number_of_tests = 15;

    double ref_qs_time; 

    uint8_t *input = calloc(input_len , 1);
    uint8_t ref_output[128];

    

    printf("\n"BHWHT"----------- Details ----------- "reset"\n");
    printf(BWHT"\nInput Length    :"CYN" %d"reset"\n", input_len);
    printf(BWHT"Output Length   :"CYN" %d"reset"\n", output_len);
    printf(BWHT"Number of Tests :"CYN" %d"reset"\n\n", number_of_tests);

    ref_qs_time = check_ref_qs(input , input_len , ref_output, number_of_tests);
    
    printf("\n"BHWHT"----------- Print Simple QuickSand ----------- "reset"\n");
    print_hex( "\nInput  : ",input , 32);
    print_hex( "Output : ",ref_output , 32);

    printf("\n\n"BHWHT"----------- Execution Time ----------- "reset"\n");
    printf(BWHT"\nREF_QS time :"CYN" %lf seconds."reset"\n\n", ref_qs_time);

    free(input);

    return 0;
}