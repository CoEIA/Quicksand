#include <quicksand.h>
#include <ref_quicksand.h>
#include <ANSI-color-codes.h>

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/time.h>

double check_ref_single_update(uint8_t *input, int input_len, uint8_t *output)
{
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    ref_qs_init();
    ref_qs_update(input, input_len);
    ref_qs_digest(output);
    ref_qs_cleanup();

    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program. 
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    return time_taken;
}

double check_ref_multiple_update(uint8_t *input, int input_len, uint8_t *output)
{
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    ref_qs_init();
    for(int i = 0; i < input_len; i++)
        ref_qs_update(input++, 1);
    ref_qs_digest(output);
    ref_qs_cleanup();

    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program. 
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    return time_taken;
}

double check_single_update(uint8_t *input, uint64_t input_len, uint8_t *output)
{
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);
    QS_CTX ctx;

    ctx = qs_init();
    qs_update(ctx, input, input_len);
    qs_digest(ctx, output);
    qs_cleanup(ctx);

    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program. 
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    return time_taken;
}

double check_multiple_update(uint8_t *input, uint64_t input_len, uint8_t *output)
{
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);
    QS_CTX ctx;

    ctx = qs_init();
    for(int i = 0; i < input_len; i++)
        qs_update(ctx, input++, 1);
    qs_digest(ctx, output);
    qs_cleanup(ctx);

    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program. 
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    return time_taken;

}

double check_ref_qs(uint8_t *input , uint64_t input_len , uint8_t *output,int number_of_tests){
    
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

double check_qs(uint8_t *input , uint64_t input_len , uint8_t *output, int number_of_tests){
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    for(int i =0; i< number_of_tests ; i++){
    QS_CTX ctx;
    ctx = qs_init();
    qs_update(ctx, input , input_len);
    qs_digest(ctx, output);
    qs_cleanup(ctx);

    }
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
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


bool assert_equal(uint8_t *a, uint8_t *b, int len){
    while(len--){
        if(a[len] != b[len]){
            return false;
        }
    }
    return true;
}

int main(){
    uint64_t input_len = 1000000;
    int output_len = 128;
    int number_of_tests = 15;

    double qs_time; 
    double ref_qs_time; 

    double ref_qs_single_time;
    double ref_qs_multiple_time;

    double qs_single_time;
    double qs_multiple_time;


    uint8_t *input = calloc(input_len , 1);
    uint8_t output[output_len];
    uint8_t ref_output[output_len];
    uint8_t single_update_output[output_len];
    uint8_t multiple_update_output[output_len];
    uint8_t ref_single_update_output[output_len];
    uint8_t ref_multiple_update_output[output_len];
    

    printf("\n"BHWHT"----------- Details ----------- "reset"\n");
    printf(BWHT"\nInput Length    :"CYN" %llu"reset"\n", input_len);
    printf(BWHT"Output Length   :"CYN" %d"reset"\n", output_len);
    printf(BWHT"Number of Tests :"CYN" %d"reset"\n\n", number_of_tests);

    qs_time = check_qs(input , input_len , output, number_of_tests);
    ref_qs_time = check_ref_qs(input , input_len , ref_output, number_of_tests);

    
    printf("\n"BHWHT"----------- Printing Simple QuickSand and Reference QuickSand ----------- "reset"\n");
    print_hex( "\nInput         : ",input , 32);
    print_hex( "QS Output     : ",output , 32);
    print_hex( "REF_QS Output : ",ref_output , 32);
    printf( "\n"BWHT"QS Execution Time     :"CYN" %lf seconds."reset"\n",qs_time);
    printf( BWHT"REF_QS Execution Time :"CYN" %lf seconds."reset"\n",ref_qs_time);

    printf("\n"BHWHT"----------- Checking QuickSand and Reference QuickSand ----------- "reset"\n");
    printf(BWHT"\nEquality :"reset" %s\n", assert_equal(output,ref_output,output_len)? GRN"Success"reset : RED"Failed"reset);


    printf("\n"BHWHT"----------- Test REF_QS Multiple Update() vs Single Update() ----------- "reset"\n");
    print_hex( "\nInput                    : ",input , 32);

     ref_qs_single_time =check_ref_single_update(input, input_len, ref_single_update_output);
     ref_qs_multiple_time =check_ref_multiple_update(input, input_len, ref_multiple_update_output);

    print_hex( "Single Update Output     : ",ref_single_update_output , 32);
    print_hex( "Multiple Update Output   : ",ref_multiple_update_output , 32);

    printf( "\n"BWHT"REF_QS Single Execution Time     :"CYN" %lf seconds."reset"\n",ref_qs_single_time);
    printf( BWHT"REF_QS Multiple Execution Time   :"CYN" %lf seconds."reset"\n",ref_qs_multiple_time);

    printf("\n"BHWHT"----------- Checking  REF_QS Single vs Multiple Update() QuickSand ----------- "reset"\n");
    printf(BWHT"\nEquality :"reset" %s\n", assert_equal(ref_single_update_output,ref_multiple_update_output,output_len)? GRN"Success"reset : RED"Failed"reset);

    printf("\n"BHWHT"----------- Test QS Multiple Update() vs Single Update() ----------- "reset"\n");
    print_hex( "\nInput                    : ",input , 32);

    qs_single_time = check_single_update(input, input_len, single_update_output);
    qs_multiple_time = check_multiple_update(input, input_len, multiple_update_output);

    print_hex( "Single Update Output     : ",single_update_output , 32);
    print_hex( "Multiple Update Output   : ",multiple_update_output , 32);
    printf( "\n"BWHT"QS Single Execution Time     :"CYN" %lf seconds."reset"\n",qs_single_time);
    printf( BWHT"QS Multiple Execution Time   :"CYN" %lf seconds."reset"\n",qs_multiple_time);

    printf("\n"BHWHT"----------- Checking QS Single vs Multiple Update() QuickSand ----------- "reset"\n");
    printf(BWHT"\nEquality :"reset" %s\n\n", assert_equal(single_update_output,multiple_update_output,output_len)? GRN"Success"reset : RED"Failed"reset);

    

    free(input);

    return 0;
}
