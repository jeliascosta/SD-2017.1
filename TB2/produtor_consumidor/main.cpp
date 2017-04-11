#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <mutex>
#include <condition_variable>

using namespace std;

#define NUM 10

void *print(void *thread_id){
  printf("sou a thread %d\n", thread_id);
  pthread_exit(NULL);
}

void *consumidor(pthread_t *thread){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada

}

void *produtor(pthread_t *thread){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada

}

int main(int Np, char *args[]){
  cout << "thread principal" << endl;

  // Inicializa as threads
  pthread_t thread;
  int erro;
  int id_thread, t;

  for(t = 0; t < NUM; t++){
    printf("Criando thread %d\n", t);
    if(erro = pthread_create(&thread, NULL, print, (void *)t)){
      printf("Erro número %d\n", erro);
      exit(-1);
    }
  }
  pthread_exit(NULL);
  return 0;
}
