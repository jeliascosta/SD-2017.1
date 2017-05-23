#include <rpc/rpc.h>
#define RPC_SVC
#include "rpc_math.h"

// Declare wrapper functions
int multiply(myoperands);
int power(myoperands);
int logn(myoperands);
int largest_prime_divisor(myoperands);

// Call wrapper functions
bool_t multiply_1_svc(myoperands *in, int *out, struct svc_req *rqstp) {
    *out = multiply(*in);
    return (TRUE);
}

bool_t power_1_svc(myoperands *in, int *out, struct svc_req *rqstp) {
    *out = power(*in);
    return (TRUE);
}

bool_t logn_1_svc(myoperands *in, int *out, struct svc_req *rqstp) {
    *out = logn(*in);
    return (TRUE);
}

bool_t largest_prime_divisor_1_svc(myoperands *in, int *out, struct svc_req *rqstp) {
    *out = largest_prime_divisor(*in);
    return (TRUE);
}
