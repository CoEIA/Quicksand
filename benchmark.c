#include <quicksand.h>
#include <ANSI-color-codes.h>

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/time.h>
#include<errno.h>

// For system information
#include<sys/utsname.h>

// OPENSSL this for SHA256 and SHA512 also it's 
 #include <openssl/evp.h>
 #include <openssl/sha.h>

// Grøstl
#include "hash_algorithms/grostl/Optimized_32bit/include/Groestl-opt.h"
// JHcompression
#include "hash_algorithms/JHcompression/Optimized_32bit/include/jh_ansi_opt32.h"
// Skein
#include "hash_algorithms/skein/include/SHA3api_ref.h"
// Echo
#include "hash_algorithms/echo/Optimized_32bit/include/echo.h"
// Blake
#include "hash_algorithms/blake/Optimized_32bit/include/blake_opt32.h"

/*

                                   ### BENCHMARK ###
--------------------------------------------------------------------------------------------
KECCA:
It's SHA256 and SHA512 in OPENSSL
--------------------------------------------------------------------------------------------
Grøstl:
Link: http://www.groestl.info/implementations.html 
Link To download code: http://www.groestl.info/Groestl.zip
--------------------------------------------------------------------------------------------
JHcompression:
Link: https://www3.ntu.edu.sg/home/wuhj/research/jh/
--------------------------------------------------------------------------------------------
Skein:
Link: https://github.com/brandondahler/retter/tree/master/Skein
--------------------------------------------------------------------------------------------
Echo:
Link: https://web.archive.org/web/20170604091329/http://csrc.nist.gov/groups/ST/hash/sha-3/Round2/submissions_rnd2.html
--------------------------------------------------------------------------------------------

Blake:
Link: https://web.archive.org/web/20170812201530/http://csrc.nist.gov/groups/ST/hash/sha-3/Round3/submissions_rnd3.html
--------------------------------------------------------------------------------------------
*/
// ******** Calculate Cycle per Byte
#define current_stamp(a) asm volatile("rdtsc" : "=a"(((unsigned int *)(a))[0]),\
"=d"(((unsigned int *)a)[1]))

/****************  Quicksand_1024  ***************/
double benchmark_quicksand(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){

    uint8_t output[128];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    // current_stamp(&current_start);

    for(int i =0; i< number_of_tests ; i++){
    QS_CTX ctx;
    ctx = qs_init();
    qs_update(ctx, input , input_len);
    qs_digest(ctx, output);
    qs_cleanup(ctx);

    }
    // current_stamp(&current_finish);
    // current_diff = current_finish - current_start;

    // *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
    
}
/****************  SHA3_256  ***************/
double benchmark_sha3_256(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    uint8_t output[32];
    unsigned int output_len;
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    //current_stamp(&current_start);

    for(int i =0; i< number_of_tests ; i++){
        const EVP_MD* algorithm = EVP_sha3_256();
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, algorithm, NULL);
        EVP_DigestUpdate(ctx, input, input_len);
        EVP_DigestFinal_ex(ctx, output, &output_len);
        EVP_MD_CTX_destroy(ctx);  
    }

    //current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    //*cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;

    return time_taken/number_of_tests;
}
/****************  SHA3_512  ***************/
double benchmark_sha3_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    uint8_t output[64];
    unsigned int output_len;
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    //current_stamp(&current_start);

    for(int i =0; i< number_of_tests ; i++){
        const EVP_MD* algorithm = EVP_sha3_512();
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, algorithm, NULL);
        EVP_DigestUpdate(ctx, input, input_len);
        EVP_DigestFinal_ex(ctx, output, &output_len);
        EVP_MD_CTX_destroy(ctx);  

    }
    //current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    //*cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}

// /****************  GROSTL_256  ***************/
double benchmark_Grostl_256(uint8_t *input, uint64_t input_len, int number_of_tests , int average_cycle , uint64_t *cycle_per_byte){
    grostl_BitSequence output[32];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    //int test_hash;
    int hash_len_in_bits = 256;

    for(int i =0; i< number_of_tests ; i++){
        grostl_Hash(hash_len_in_bits ,input ,input_len , output);  
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  GROSTL_512  ***************/
double benchmark_Grostl_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    grostl_BitSequence output[64];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    //int test_hash;
    int hash_len_in_bits = 512;

    for(int i =0; i< number_of_tests ; i++){
        grostl_Hash(hash_len_in_bits ,input ,input_len , output);  
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}

// /****************  JHcompression_256  ***************/
double benchmark_JHcompression_256(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    JHcompression_BitSequence output[32];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int hash_len_in_bits = 256;

    for(int i =0; i< number_of_tests ; i++){
        JHcompression_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  JHcompression_512  ***************/
double benchmark_JHcompression_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    JHcompression_BitSequence output[64];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int hash_len_in_bits = 512;

    for(int i =0; i< number_of_tests ; i++){
        JHcompression_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Skein_256  ***************/
double benchmark_Skein_256(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    skein_BitSequence output[32];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int hash_len_in_bits = 256;

    for(int i =0; i< number_of_tests ; i++){
        skein_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Skein_512  ***************/
double benchmark_Skein_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    skein_BitSequence output[64];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int hash_len_in_bits = 512;

    for(int i =0; i< number_of_tests ; i++){
        skein_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Skein_1024  ***************/
double benchmark_Skein_1024(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    skein_BitSequence output[128];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int hash_len_in_bits = 1024;

    for(int i =0; i< number_of_tests ; i++){
        skein_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
    // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Echo_256  ***************/
double benchmark_Echo_256(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    echo_BitSequence output[32];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int test_hash;
    int hash_len_in_bits = 256;
    for(int i =0; i< number_of_tests ; i++){ 
        test_hash = echo_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Echo_512  ***************/
double benchmark_Echo_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    echo_BitSequence output[64];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int test_hash;
    int hash_len_in_bits = 512;
    for(int i =0; i< number_of_tests ; i++){ 
        test_hash = echo_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Blake_256  ***************/
double benchmark_Blake_256(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    echo_BitSequence output[32];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int test_hash;
    int hash_len_in_bits = 256;
    for(int i =0; i< number_of_tests ; i++){ 
        test_hash = blake_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
// /****************  Blake_512  ***************/
double benchmark_Blake_512(uint8_t *input, uint64_t input_len, int number_of_tests, int average_cycle , uint64_t *cycle_per_byte){
    echo_BitSequence output[64];
    struct timeval start, end;
    uint64_t current_start, current_finish, current_diff;

    // start timer.
    gettimeofday(&start, NULL);
    current_stamp(&current_start);

    int test_hash;
    int hash_len_in_bits = 512;
    for(int i =0; i< number_of_tests ; i++){ 
        test_hash = blake_Hash(hash_len_in_bits ,input ,input_len , output);
    }
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;

    *cycle_per_byte = (current_diff - average_cycle)/(input_len * (uint64_t)number_of_tests);
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}
 /****************  Claculate Average CPU Clock Cycle with Empty Opreation   ***************/
uint64_t CPU_Average(int number_of_tests){
    uint64_t current_start, current_finish, current_diff;
    uint64_t total_cycle = 0;

    for(int i = 0; i < number_of_tests; i++){
    current_stamp(&current_start);
    current_stamp(&current_finish);
    current_diff = current_finish - current_start;
    total_cycle += current_diff;
    }
    return total_cycle/number_of_tests;
}


void table_format(){
    printf(BHYEL"\n---------------------------------------------------------\n\n"reset);
}

int main(){
    uint64_t KB = 1024;
    uint64_t MB = 1024 * KB;

    uint64_t input_len[9] = {1*KB , 4*KB , 8*KB, 16*KB, 1*MB, 4*MB, 8*MB, 16*MB, 1*1024*MB};
    int number_of_tests = 5;

    uint64_t average_cycle;

    double sha256_time;
    double sha512_time; 
    uint64_t sha256_cycle_per_byte;
    uint64_t sha512_cycle_per_byte;

    double groestl256_time; 
    double groestl512_time; 
    uint64_t groestl256_cycle_per_byte;
    uint64_t groestl512_cycle_per_byte;

    double JHcompression256_time; 
    double JHcompression512_time;
    uint64_t JHcompression256_cycle_per_byte;
    uint64_t JHcompression512_cycle_per_byte; 

    double skein256_time; 
    double skein512_time; 
    double skein1024_time;
    uint64_t skein256_cycle_per_byte;
    uint64_t skein512_cycle_per_byte; 
    uint64_t skein1024_cycle_per_byte; 

    double echo256_time; 
    double echo512_time;
    uint64_t echo256_cycle_per_byte;
    uint64_t echo512_cycle_per_byte;

    double blake256_time; 
    double blake512_time;
    uint64_t blake256_cycle_per_byte;
    uint64_t blake512_cycle_per_byte;

    double qs_time; 
    uint64_t qs_cycle_per_byte;
 

    // *** For CPU info see this link: https://github.com/pytorch/cpuinfo/tree/master/include ****
    printf(BBLU"\n******** System Informations ********\n\n"reset);
    struct utsname sys_info;
    errno =0;
    if(uname(&sys_info)!=0)
    {
        perror(RED"uname doesn't return 0, so there is an error\n"reset);
        exit(EXIT_FAILURE);
    }
    #if __APPLE__
	    // apple specific code
        printf(BWHT"OS Name     = "CYN"MacOS\n"reset);
	#elif _WIN32
        #ifdef _WIN64
        // windows specific code
            printf(BWHT"OS Name     = "CYN"Windows 64 bit\n"reset);
	    #else 
            printf(BWHT"OS Name     = "CYN"Windows 32 bit\n"reset);
	    #endif      
	#elif __LINUX__
	    // linux specific code
        printf(BWHT"OS Name     = "CYN"Linux\n"reset);
    #elif __unix__
	    // linux specific code
        printf(BWHT"OS Name     = "CYN"Unix\n"reset);
	#elif BSD
	    // BSD specific code
        printf(BWHT"OS Name     = "CYN"BSD\n"reset);
	#else
	    // general code or warning
        printf(BWHT"OS Name     = "CYN"Can't detected\n"reset);
	#endif
   
    printf(BWHT"System Name = "CYN"%s\n"reset, sys_info.sysname);
    printf(BWHT"Version     = "CYN"%s\n"reset, sys_info.version);
    printf(BWHT"Release     = "CYN"%s\n"reset, sys_info.release);
    printf(BWHT"Machine     = "CYN"%s\n"reset, sys_info.machine);

    // average_cycle = CPU_Average(number_of_tests);
    // printf(BWHT"Average CPU Clock Cycle with Nothing     = "CYN"%lld\n"reset, average_cycle);

    table_format();

    printf(BBLU"******** Benchmark ********\n"reset);
    //printf(WHT"Input Size: "BLU"%llu GB"reset, input_len[8]/(1024*1024*1024));
    printf(WHT"Number of test: "CYN"%d\n"reset, number_of_tests);

    printf("\n");
    printf(BGRN"Staring ....\n"reset);

    for(int i=0; i< 9; i++){ // loop for sizes
        uint8_t *input = calloc(1, input_len[i]);
        table_format();
        if((i == 0) || (i == 1) || (i == 2) || (i == 3)){
            printf(BWHT"Input Size: "BCYN"%llu KB\n\n"reset, input_len[i]/(1024));
        }
        if((i == 4) || (i == 5) || (i == 6) || (i == 7)){
            printf(BWHT"Input Size: "BCYN"%llu MB\n\n"reset, input_len[i]/(1024*1024));
        }
        if(i == 8){
            printf(BWHT"Input Size: "BCYN"%llu GB\n\n"reset, input_len[i]/(1024*1024*1024));
        }
        //continue;
        // ****************** SHA3 256,512 ******************
        sha256_time = benchmark_sha3_256(input, input_len[i], number_of_tests , average_cycle , &sha256_cycle_per_byte);
        printf(BWHT"Sha3_256 time: \t\t\t\t"GRN"%f\tseconds"reset"\n", sha256_time);
        printf(BWHT"Sha3_256 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", sha256_cycle_per_byte);
        sha512_time = benchmark_sha3_512(input, input_len[i], number_of_tests , average_cycle , &sha512_cycle_per_byte);
        printf(BWHT"Sha3_512 time: \t\t\t\t"GRN"%f\tseconds"reset"\n", sha512_time);
        printf(BWHT"Sha3_512 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", sha512_cycle_per_byte);

        // ****************** QUICKSAND 1024 ******************
        qs_time = benchmark_quicksand(input, input_len[i], number_of_tests ,average_cycle , &qs_cycle_per_byte);
        printf(BWHT"Quicksand_1024 time: \t\t\t"GRN"%f\tseconds"reset"\n", qs_time);
        printf(BWHT"Quicksand_1024 Cycle/Byte: \t\t"GRN"%lld\t\tCycle/Byte"reset"\n", qs_cycle_per_byte);

        // ****************** GROSTL 256,512 ******************
        groestl256_time = benchmark_Grostl_256(input, input_len[i], number_of_tests ,average_cycle , &groestl256_cycle_per_byte);
        printf(BWHT"Groestl_256 time: \t\t\t"GRN"%f\tseconds"reset"\n", groestl256_time);
        printf(BWHT"Groestl_256 Cycle/Byte: \t\t"GRN"%lld\t\tCycle/Byte"reset"\n", groestl256_cycle_per_byte);

        groestl512_time = benchmark_Grostl_512(input, input_len[i], number_of_tests ,average_cycle , &groestl512_cycle_per_byte);
        printf(BWHT"Groestl_512 time: \t\t\t"GRN"%f\tseconds"reset"\n", groestl512_time);
        printf(BWHT"Groestl_512 Cycle/Byte: \t\t"GRN"%lld\t\tCycle/Byte"reset"\n", groestl512_cycle_per_byte);


        // ****************** JH-COMPRESSION 256,512 ******************
        JHcompression256_time = benchmark_JHcompression_256(input, input_len[i], number_of_tests ,average_cycle , &JHcompression256_cycle_per_byte);
        printf(BWHT"JHcompression_256 time: \t\t"GRN"%f\tseconds"reset"\n", JHcompression256_time);
        printf(BWHT"JHcompression_256 Cycle/Byte: \t\t"GRN"%lld\t\tCycle/Byte"reset"\n", JHcompression256_cycle_per_byte);

        JHcompression512_time = benchmark_JHcompression_512(input, input_len[i], number_of_tests ,average_cycle , &JHcompression512_cycle_per_byte);
        printf(BWHT"JHcompression_512 time: \t\t"GRN"%f\tseconds"reset"\n", JHcompression512_time);
        printf(BWHT"JHcompression_512 Cycle/Byte: \t\t"GRN"%lld\t\tCycle/Byte"reset"\n", JHcompression512_cycle_per_byte);


        // ****************** SKEIN 256,512,1024 ******************
        skein256_time = benchmark_Skein_256(input, input_len[i], number_of_tests ,average_cycle , &skein256_cycle_per_byte);
        printf(BWHT"Skein_256 time: \t\t\t"GRN"%f\tseconds"reset"\n", skein256_time);
        printf(BWHT"Skein_256 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", skein256_cycle_per_byte);

        skein512_time = benchmark_Skein_512(input, input_len[i], number_of_tests ,average_cycle , &skein512_cycle_per_byte);
        printf(BWHT"Skein_512 time: \t\t\t"GRN"%f\tseconds"reset"\n", skein512_time);
        printf(BWHT"Skein_512 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", skein512_cycle_per_byte);

        skein1024_time = benchmark_Skein_1024(input, input_len[i], number_of_tests ,average_cycle , &skein1024_cycle_per_byte);
        printf(BWHT"Skein_1024 time: \t\t\t"GRN"%f\tseconds"reset"\n", skein1024_time);
        printf(BWHT"Skein_1024 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", skein1024_cycle_per_byte);

        // ****************** Echo 256,512 ******************
        echo256_time = benchmark_Echo_256(input, input_len[i], number_of_tests ,average_cycle , &echo256_cycle_per_byte);
        printf(BWHT"Echo_256 time: \t\t\t\t"GRN"%f\tseconds"reset"\n", echo256_time);
        printf(BWHT"Echo_256 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", echo256_cycle_per_byte);
       
        echo512_time = benchmark_Echo_512(input, input_len[i], number_of_tests ,average_cycle , &echo512_cycle_per_byte);
        printf(BWHT"Echo_512 time: \t\t\t\t"GRN"%f\tseconds"reset"\n", echo512_time);
        printf(BWHT"Echo_512 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", echo512_cycle_per_byte);

        // ****************** Blake 256,512 ******************
        blake256_time = benchmark_Blake_256(input, input_len[i], number_of_tests ,average_cycle , &blake256_cycle_per_byte);
        printf(BWHT"Blake_256 time: \t\t\t"GRN"%f\tseconds"reset"\n", blake256_time);
        printf(BWHT"Blake_256 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", blake256_cycle_per_byte);
       
        blake512_time = benchmark_Blake_512(input, input_len[i], number_of_tests ,average_cycle , &blake512_cycle_per_byte);
        printf(BWHT"Blake_512 time: \t\t\t"GRN"%f\tseconds"reset"\n", blake512_time);
        printf(BWHT"Blake_512 Cycle/Byte: \t\t\t"GRN"%lld\t\tCycle/Byte"reset"\n", blake512_cycle_per_byte);

    }

    return 0;
}