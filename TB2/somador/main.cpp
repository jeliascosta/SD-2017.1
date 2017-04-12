#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define NUM_THREADS 2
#define N 10

using namespace std;

// Variáveis globais
int *buffer;

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

void* contar(void *id){

}

int main(int argc, char *argv[]){

  buffer = (int *)malloc(N * sizeof(int));
  if(buffer == NULL){
    cout << "Falha ao alocar memória" << endl;
    return EXIT_FAILURE;
  }

  // Cria N threads
  pthread_t Threads[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&Threads[i], NULL, contar, (void *)i);
  }

  return 0;
}
