#include "include/Groestl-opt.h"
#include "include/ANSI-color-codes.h"

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/time.h>



// double benchmark_quicksand(uint8_t *input, uint64_t input_len, int number_of_tests){

//     uint8_t output[128];
//     struct timeval start, end;
//     // start timer.
//     gettimeofday(&start, NULL);

//     for(int i =0; i< number_of_tests ; i++){
//     QS_CTX ctx;
//     ctx = qs_init();
//     qs_update(ctx, input , input_len);
//     qs_digest(ctx, output);
//     qs_cleanup(ctx);

//     }
//      // stop timer.
//     gettimeofday(&end, NULL);
//     // Calculating total time taken by the program.
//     double time_taken = 0;
//     time_taken = (end.tv_sec - start.tv_sec) * 1e6;
//     time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
//     return time_taken/number_of_tests;
    
// }

// double benchmark_sha3_256(uint8_t *input, uint64_t input_len, int number_of_tests){
//     uint8_t output[32];
//     unsigned int output_len;
//     struct timeval start, end;
//     // start timer.
//     gettimeofday(&start, NULL);

//     for(int i =0; i< number_of_tests ; i++){
//         const EVP_MD* algorithm = EVP_sha3_256();
//         EVP_MD_CTX* ctx = EVP_MD_CTX_new();
//         EVP_DigestInit_ex(ctx, algorithm, NULL);
//         EVP_DigestUpdate(ctx, input, input_len);
//         EVP_DigestFinal_ex(ctx, output, &output_len);
//         EVP_MD_CTX_destroy(ctx);  

//     }
//      // stop timer.
//     gettimeofday(&end, NULL);
//     // Calculating total time taken by the program.
//     double time_taken = 0;
//     time_taken = (end.tv_sec - start.tv_sec) * 1e6;
//     time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
//     return time_taken/number_of_tests;
// }

double benchmark_Grostl_256(uint8_t *input, uint64_t input_len, int number_of_tests){
    BitSequence output[32];
    unsigned int output_len;
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    int test_hash;
    int hash_len_in_bits = 256;
    hashState* ctx;

    for(int i =0; i< number_of_tests ; i++){
        
        test_hash = Hash(hash_len_in_bits ,input ,input_len , output);
        if(test_hash == 0){   
            printf(BWHT"\nThe Hashing Hash() process:"GRN" SUCCESS"reset" \n");
        }else if(test_hash == 1){
            printf(BWHT"\nThe Hashing Hash() process:"RED" Fail"reset" \n");
            exit(1);
        }else if(test_hash == 2){
            printf(BWHT"\nThe Hashing Hash() process:"RED" BAD HASHLEN"reset" \n");
            exit(1);
        }
    }
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;
    return time_taken/number_of_tests;
}

int main(){
    uint64_t KB = 1024;
    uint64_t MB = 1024 * KB;

    uint64_t input_len[9] = {1*KB , 4*KB , 8*KB, 16*KB, 1*MB, 4*MB, 8*MB, 16*MB, 1*1024*MB};
    int number_of_tests = 1;

    double qs_time; 
    double sha512_time; 
    double sha256_time; 
    double groestl_time; 

    printf(BBLU"******** Benchmark ********\n"reset);
    //printf(WHT"Input Size: "BLU"%llu GB"reset, input_len[8]/(1024*1024*1024));
    printf(WHT"\nNumber of test: "BLU"%d"reset, number_of_tests);

    printf("\n\n");
    printf(BGRN"Staring ....\n"reset);
    printf("\n");

    for(int i=8; i< 9; i++){
        uint8_t *input = calloc(1, input_len[i]);
        printf(BWHT"Input Size: "BCYN"%llu GB"reset"\n", input_len[i]/(1024*1024*1024));
        
        // qs_time = benchmark_quicksand(input, input_len[i], number_of_tests);
        // printf("Quicksand time: "GRN"%f"reset"\n", qs_time);
        // sha256_time = benchmark_sha3_256(input, input_len[i], number_of_tests);
        // printf("Sha3_256  time: "GRN"%f"reset"\n", sha256_time);
        // sha512_time = benchmark_sha3_512(input, input_len[i], number_of_tests);
        // printf("Sha3_512  time: "GRN"%f"reset"\n", sha512_time);

        groestl_time = benchmark_Grostl_256(input, input_len[i], number_of_tests);
        printf("groestl_256  time: "GRN"%f"reset"\n", groestl_time);

        printf("------------------------\n");

    }

    return 0;
}