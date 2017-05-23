struct myoperands {
    int arr<>;
    int argument;
};

program RPCMATHPROG {
    version RPCMATHVERS {
    int MULTIPLY(myoperands)=1;
    int POWER(myoperands)=2;
    int LOGN(myoperands)=3;
    int LARGEST_PRIME_DIVISOR(myoperands)=4;
    } =1;
} =0x30090948;
