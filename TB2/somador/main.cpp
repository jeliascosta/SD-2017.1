#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define NUM_THREADS 2
#define N 10 //Tamanho do array

using namespace std;

// Variáveis globais
int *buffer;

pthread_mutex_t randMutex;

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

void generate_buffer(){
  for(int i = 0; i < N; i++){
    int random = rand() % 201 + (-100);
    buffer[i] = random;
  }
}

void* somador(void *id){

}

int main(int argc, char *argv[]){

  buffer = (int *)malloc(N * sizeof(int));
  if(buffer == NULL){
    cout << "Falha ao alocar memória" << endl;
    return EXIT_FAILURE;
  }

  srand(time(NULL));
  // Cria N threads
  // pthread_t Threads[NUM_THREADS];
  // for(int i = 0; i < NUM_THREADS; i++){
  //   pthread_create(&Threads[i], NULL, somador, (void *)i);
  // }

  return 0;
}
