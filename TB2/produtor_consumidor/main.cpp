#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>


using namespace std;

#define NUM 10
#define SIZE 10
#define ALEATORY 100

void *print(void *thread_id){
  printf("sou a thread %d\n", thread_id);
  pthread_exit(NULL);
}

void *consumidor(pthread_t *thread){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada

}

void *produtor(){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada
  // GERAR NÚMEROS ALEATÓRIOS
  srand(time(NULL));
  int aleatory_number = rand() % ALEATORY + 1;
  cout << aleatory_number << endl;

}

void inicializar_memoria(int M){
  int *buffer = NULL;
  buffer = new int[M];
  for(int i = 0; i < M; i++){
    buffer[i] = 0;
  }
  delete [] buffer;
  buffer = NULL;
}

int main(int Np, char *args[]){
  cout << "Produtor Consumidor" << endl;
  // ================== INICIALIZANDO O BUFFER =================
  int *buffer = NULL;
  buffer = new int[SIZE];
  for(int i = 0; i < SIZE; i++){
    buffer[i] = 0;
  }
  produtor();
  // Inicializa as threads
  pthread_t thread;
  int erro;
  int id_thread, t;

  for(t = 0; t < NUM; t++){
    printf("Criando thread %d\n", t);
    if(pthread_create(&thread, NULL, print, (void *)t) != 0){
      printf("Erro número %d\n", erro);
      exit(-1);
    }
  }
  pthread_exit(NULL);
  // ================ LIBERANDO A MEMÓRIA UTILIZADA NO BUFFER ==============
  delete [] buffer;
  buffer = NULL;
  return 0;
}
