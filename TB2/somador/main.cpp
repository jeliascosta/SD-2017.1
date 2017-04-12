#include <iostream>
#include <atomic>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define NUM_THREADS 3
#define N 10 //Tamanho do array

using namespace std;

// Variáveis globais
atomic_flag lock_stream = ATOMIC_FLAG_INIT;
int *buffer; //buffer com os valores de -100 até 100
int acumulador = 0; //variável usada para guardar a soma do valores do buffer


void acquire(){
  while(lock_stream.test_and_set());
}

void release(){
  lock_stream.clear();
}

void generate_buffer(){
  for(int i = 0; i < N; i++){
    int random = rand() % 10;
    buffer[i] = random;
  }

  for(int i = 0; i < N; i++){
    cout << buffer[i] << " ";
  }
}

void* somador(void *ID){
  int id  = (intptr_t)ID;
  int soma_local = 0;
  // Dividir o array
  int offset = N%NUM_THREADS;
  int chunks = floor(N/NUM_THREADS);
  int start =id*chunks;
  int end = ((id+1)*chunks);  
  for(int i = start; i < end; i++){
    soma_local += buffer[i];
  }
  acquire();
  acumulador += soma_local;
  release();
}

int main(int argc, char *argv[]){

  buffer = (int *)malloc(N * sizeof(int));
  if(buffer == NULL){
    cout << "Falha ao alocar memória" << endl;
    return EXIT_FAILURE;
  }
  generate_buffer();

  srand(time(NULL));
  // Cria N threads
  pthread_t Threads[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&Threads[i], NULL, somador, (void *)i);
  }

  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(Threads[i], NULL);
  }
  cout << "encerramento do programa" << endl;
  cout << "soma final: " << acumulador << endl;

  return 0;
}
