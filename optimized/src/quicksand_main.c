#include <quicksand.h>
#include <ANSI-color-codes.h>

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include <sys/time.h>

#include <errno.h>
#include <string.h>

#include <pthread.h>
#include <unistd.h>


#ifndef NUM_THREADS
#define NUM_THREADS 3
#endif

struct arg_struct {
    uint64_t input_len;
    int output_len;
    int number_of_tests;
    double qs_time_thread; 

    uint8_t *input;
    uint8_t output[128];

    long thread_id;
};

/**
 * @brief check_qa function this fuction to test the Quicksand
 * 
 * @param arguments The struct arguments
 * @return void* 
 */
void *check_qs(void *arguments){
    struct arg_struct *args = (struct arg_struct *)arguments;
    long tid;
    tid = (long)args->thread_id;
    
    struct timeval start, end;
    // start timer.
    gettimeofday(&start, NULL);

    for(int i =0; i< args->number_of_tests ; i++){
    QS_CTX ctx;
    ctx = qs_init();
    qs_update(ctx, args->input , args->input_len);
    qs_digest(ctx, args->output);

    qs_cleanup(ctx);
    }
     // stop timer.
    gettimeofday(&end, NULL);
    // Calculating total time taken by the program.
    double time_taken = 0;
    time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6;

    args->qs_time_thread = time_taken/args->number_of_tests;

    printf("Thread["GRN"%ld"reset"]"reset, tid);
    pthread_exit(NULL);
    return NULL;
    
}

/**
 * @brief Printing sample of the Output
 * 
 * @param str Take the a string.
 * @param output Take the output
 * @param len Length to be prented. How many byte you want to print.
 */
void print_hex(char *str,uint8_t *output , int len){
    printf(BWHT"%s"reset, str);
    printf(CYN);
    for(int i =0; i < len ; i++){
        printf("%02X", output[i]);
    }
    printf(reset"\n");

}


int main(){

    struct arg_struct args[NUM_THREADS];

    // threads
    pthread_t threads[NUM_THREADS];
    int rc;


    for(int t = 0; t < NUM_THREADS ; t++){
        args[t].input_len = 2048000*10;
        args[t].output_len = 128;
        args[t].number_of_tests = 1;

        args[t].input = calloc(1, args[t].input_len);
        if(args[t].input == NULL){
            printf(RED"\nCalloc filed"reset);

            printf("\n%s: %llu", strerror(errno),args[t].input_len);
            exit(1);
        }
        args[t].thread_id = t;
        rc = pthread_create(&threads[t], NULL, *check_qs, (void *)&args[t]);
        if (rc) {
            printf(REDB"ERORR"reset"; return code from pthread_create() is %d\n", rc);
            exit(EXIT_FAILURE);
        }

    }
    
    for(int t = 0; t < NUM_THREADS ; t++)
        pthread_join(threads[t], NULL);
    
    for(int t = 0; t < NUM_THREADS ; t++){
        printf("\n"BHWHT"----------- Print Simple QuickSand ----------- "reset"\n");
        print_hex( "\nInput  : ",args[t].input , 32);
        print_hex( "Output : ",args[t].output , 32);

        printf("\n\n"BHWHT"----------- Execution Time ----------- "reset"\n");
        printf(BWHT"\nQS time     :"CYN" %lf seconds."reset"\n\n", args[t].qs_time_thread);

        free(args[t].input);
    }
    

    return 0;
}