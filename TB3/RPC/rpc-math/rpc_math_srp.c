#include <rpc/rpc.h>
#include <math.h>
#include "rpc_math.h"

int multiply(myoperands op) {
    // Unpack operands
    unsigned int size = op.arr.arr_len;
    int *data = op.arr.arr_val;
    int arg = op.argument;
    int result;

    printf("Processing request...\n");

    // Operate on arrays
    unsigned int i;
    for (i = 0; i < size; i++) {
        result = data[i] * arg;
    }

    printf("Returning...\n");

    // Return ok
    return 1;
}

int power(myoperands op) {
    // Unpack operands
    unsigned int size = op.arr.arr_len;
    int *data = op.arr.arr_val;
    int arg = op.argument;
    int result;

    printf("Processing request...\n");

    // Operate on arrays
    unsigned int i;
    for (i = 0; i < size; i++) {
        result = (int) pow(data[i], arg);
    }

    printf("Returning...\n");

    // Return ok
    return 1;
}

int logn(myoperands op) {
    // Unpack operands
    unsigned int size = op.arr.arr_len;
    int *data = op.arr.arr_val;
    int base = op.argument;
    double logbase = log(base);
    int result;

    printf("Processing request...\n");

    // Operate on arrays
    unsigned int i;
    for (i = 0; i < size; i++) {
        result = (int) (log(data[i]) / logbase);
    }

    printf("Returning...\n");

    // Return ok
    return 1;
}

int largest_prime_divisor(myoperands op) {
    // Unpack operands
    unsigned int size = op.arr.arr_len;
    int *data = op.arr.arr_val;
    int arg = op.argument;
    int result;

    printf("Processing request...\n");

    // Operate on arrays
    unsigned int i, j;
    int value, max;
    for (i = 0; i < size; i++) {
        // Take number
        value = data[i];
        // If 1, it is already its largest "prime"
        if (value == 1) {
            result = 1;
            continue;
        }
        // Iterate numbers, dividing value by primes
        for (j = 2; j * j < value; i++) {
            while (value % j == 0) {
                // Found prime divisor, divide and update max
                value /= j;
                max = j;
            }
        }
        // If value was prime, it will still be > 1, so the answer is itself
        if (value > 1) {
            result = data[i];
        }
        else {
            result = max;
        }
    }

    printf("Returning...\n");

    // Return ok
    return 1;
}

rpcmathprog_1_freeresult(SVCXPRT *transp,xdrproc_t xdr_result, caddr_t result) {
    xdr_free(xdr_result,result);
    return 1;
}
