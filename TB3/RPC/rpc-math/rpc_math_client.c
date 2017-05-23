#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include "rpc_math.h"
#include "string.h"

#define F_MULTIPLY 0
#define F_POWER 1
#define F_LOGBASEN 2
#define F_PRIME 3

// Information to be passed along to each thread
typedef struct thread_data{
    unsigned int thread_id; // Thread ID
    unsigned int seed; // RNG seed
    unsigned int iterations; // How many elements to generate for server
    unsigned short func; // Which function to call
    int func_arg; // Function argument
    char *host; // Connection host
} thread_data;

// Generate number on interval 1-10000
int genRand(unsigned int *seed) {
    int value = rand_r(seed);
    value = (int) (value / ((double) RAND_MAX));
    value *= 9999;
    value += 1;
    return value;
}

// Code to be executed by each thread - calls RPC function
void *threadRPC(void *arg){
    // Get thread data pointer
    thread_data *t_data = (thread_data *)arg;

    // Unpack variables
    unsigned int seed = t_data->seed;
    unsigned int iterations = t_data->iterations;
    unsigned short func = t_data->func;
    int func_arg = t_data->func_arg;
    char *host = t_data->host;

    // Call memory to be used through RPC
    int *data = (int *) malloc(iterations*sizeof(int));

    // Generate random values
    unsigned int i = 0;
    for (i = 0; i < iterations; i++) {
        data[i] = genRand(&seed);
    }

    // Operands to and from server structures
    myoperands toServer;
    int fromServer;

    // Set send operand
    toServer.arr.arr_len = iterations;
    toServer.arr.arr_val = data;
    toServer.argument = func_arg;

    // Initialize RPC variables
    CLIENT *clnt;
    enum clnt_stat retval;
    clnt = clnt_create(host, RPCMATHPROG, RPCMATHVERS, "tcp");
    if (clnt == NULL) {
        printf("Error creating RPC client\n");
        pthread_exit(NULL);
    }

    printf("Calling function...\n");

    switch(func) {
    case F_MULTIPLY:
        retval = multiply_1(&toServer, &fromServer, clnt);
        break;
    case F_POWER:
        retval = power_1(&toServer, &fromServer, clnt);
        break;
    case F_LOGBASEN:
        retval = logn_1(&toServer, &fromServer, clnt);
        break;
    case F_PRIME:
        retval = largest_prime_divisor_1(&toServer, &fromServer, clnt);
        break;
    }

    printf("Received reply!\n");

    // free(data); ??

    if (retval != RPC_SUCCESS) {
        printf("Error at server during RPC call\n");
        pthread_exit(NULL);
    }

    // free(fromServer.arr.arr_val); ??

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    // Get RNG seed from time
    unsigned int seed = time(NULL);

    unsigned long n_numbers;
    unsigned int n_threads;
    unsigned int func_id;
    int func_arg;
    char *host;

    // Parse arguments
    if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))){
        printf("Usage: ./app_client <HOST> <N> <K> <FUNC> <ARG>\n"
               "    HOST: IP to connect to\n"
               "       N: Number of numbers to generate\n"
               "       K: Number of threads to use\n"
               "    FUNC: Function id to use [0-3]\n"
               "     ARG: Function argument (optional, defaults 0)\n");
        exit(0);
    }

    else if (argc > 6 || argc < 5){
        printf("Arguments specified wrongly. Use --help for usage.\n");
        exit(1);
    }

    else if (argc == 6) {
        func_arg = atoi(argv[5]);
    }

    else {
        func_arg = 0;
    }

    host = argv[1];
    n_numbers = atol(argv[2]);
    n_threads = atoi(argv[3]);
    func_id = atoi(argv[4]);

    if (func_id < 0 || func_id > 3) {
        printf("Invalid function id.\n");
        exit(1);
    }

    // Configure pthread - number of threads
    pthread_t threads[n_threads];

    // Start thread data array - on heap so threads can access it
    thread_data *thread_data_array;
    thread_data_array = (thread_data *)malloc(sizeof(thread_data) * n_threads);
    if (!thread_data_array){
        printf("Not enough memory\n");
        exit(1);
    }

    // Configure thread attributes - force to be joinable
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Compute thread load, initialize threads
    int rc;
    unsigned int i, t, load, rest;
    void *status;

    load = n_numbers / n_threads;
    rest = n_numbers - (load * n_threads);

    for (t = 0; t < n_threads; t++){
        // Initialize thread data for this thread
        thread_data_array[t].thread_id = t; // Id is current iteration
        thread_data_array[t].seed = seed; // RNG seed
        thread_data_array[t].func = func_id; // Function id
        thread_data_array[t].func_arg = func_arg; // Function argument
        thread_data_array[t].host = host; // Connection host
        if (t == (n_threads - 1)){
            // Last thread should work slightly more to cover for uneven division
            thread_data_array[t].iterations = load + rest;
        }
        else {
            thread_data_array[t].iterations = load;
        }
        rc = pthread_create(&threads[t], &attr, threadRPC, (void *) &thread_data_array[t]);
        if (rc){
            printf("Could not create thread\n");
            exit(1);
        }
    }

    // Wait for all threads to join back
    for (t = 0; t < n_threads; t++){
        rc = pthread_join(threads[t], &status);
        if (rc){
            printf("Thread returned an error\n");
            exit(1);
        }
    }

    // Free memory
    pthread_attr_destroy(&attr);
    free(thread_data_array);

    printf("DONE!\n");

    return 0;
}
