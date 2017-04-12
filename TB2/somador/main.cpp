#include <iostream>
#include <atomic>
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
atomic_flag lock_stream = ATOMIC_FLAG_INIT;
int *buffer; //buffer com os valores de -100 até 100
int acumulador; //variável usada para guardar a soma do valores do buffer


void acquire(){
  while(lock_stream.test_and_set());
}

void release(){
  lock_stream.clear();
}

void generate_buffer(){
  for(int i = 0; i < N; i++){
    int random = rand() % 201 + (-100);
    buffer[i] = random;
  }
}

void* somador(void *id){

  acquire();
  release();
}

int main(int argc, char *argv[]){

  buffer = (int *)malloc(N * sizeof(int));
  if(buffer == NULL){
    cout << "Falha ao alocar memória" << endl;
    return EXIT_FAILURE;
  }

  srand(time(NULL));
  // Cria N threads
  pthread_t Threads[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&Threads[i], NULL, somador, (void *)i);
  }

  return 0;
}
