#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0

using namespace std;

struct LOCK{
  bool held = FALSE;
};

bool test_and_set(bool *flag){
  bool anterior = *flag;
  *flag = TRUE;
  return anterior;
}

void acquire(LOCK *lock){
  while(test_and_set(&lock->held));
}

void release(LOCK *lock){
  lock->held = FALSE;
}

int main(int argc, char *argv[]){

  /

  return 0;
}
