// Para compilar o programa, utilizar a versão 11 do c++
//Comando: g++ -std=c++11 main.cpp

#include <iostream>
#include <atomic>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <chrono>

#define NUM_THREADS 128 //Número de threads
#define N 10000000 //Tamanho do array

using namespace std;

// Variáveis globais
atomic_flag lock_stream = ATOMIC_FLAG_INIT;
int *buffer; //buffer com os valores de -100 até 100
int acumulador = 0; //variável usada para guardar a soma do valores do buffer


// Função acquire usando test_and_set com busy_wait
void acquire(){
  while(lock_stream.test_and_set());
}

// Função release usando test_and_set
void release(){
  lock_stream.clear();
}

// Função para gerar um buffer com números aleatórios
void generate_buffer(){
  int sum = 0;
  for(int i = 0; i < N; i++){
    int random = rand() % 201 + (-100);
    buffer[i] = random;
  }

  for(int i = 0; i < N; i++){
    sum += buffer[i];
  }
  cout << endl;
  // Salva o valor da soma para conferência de corretude do programa
  cout << "soma: " << sum;
}

// Função usada por cada thread para somar seus números
void* somador(void *ID){
  int id  = (intptr_t)ID;
  int soma_local = 0;
  // Dividir o array
  int offset = N%NUM_THREADS; // números que estão dos "pacotes" para cada thread
  int chunks = floor(N/NUM_THREADS);
  int start =id*chunks;
  int end = ((id+1)*chunks);

  // Somar os números restantes que estão fora dos pacotes de cada thread na última thread
  if((N-end) <= chunks){
    end = end+offset;
  }
  for(int i = start; i < end; i++){
    soma_local += buffer[i];
  }
  // Início da região crítica
  acquire();
  acumulador += soma_local;
  release();
  // Fim da região crítica
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
  struct timespec start, finish;
  double elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
  auto begin = chrono::high_resolution_clock::now();

  pthread_t Threads[NUM_THREADS];
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&Threads[i], NULL, somador, (void *)i);
  }

  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(Threads[i], NULL);
  }

  auto end = chrono::high_resolution_clock::now();

  clock_gettime(CLOCK_MONOTONIC, &finish);

  cout.flush();
  cout << "\n\nTempo de execução pela C++11 chrono:\t" << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;

  elapsed = ((finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0) * (1000000000);
  cout << "\n\nTempo de execução pela POSIX time:\t" << elapsed << "ns" << endl;

  cout << "soma final: " << acumulador << endl;

  return 0;
}
