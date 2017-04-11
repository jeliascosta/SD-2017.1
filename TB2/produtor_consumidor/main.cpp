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

int consumidor(int *buffer, int size){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada
  // PERCORRE O VETOR EM BUSCA DE UMA POSIÇÃO DIFERENTE DE 0
  for(int i = 0; i < size; i++){
    if(buffer[i] != 0){//VERIFICA SE O NÚMERO É PRIMO E RETORNA
      cout << buffer[i] << endl;
      buffer[i] = 0;
      return 1;
    }else{//RETORNA UM ERRO
      return -1;
    }
  }
}

int produtor(int *buffer, int size){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada
  // GERAR NÚMEROS ALEATÓRIOS
  srand(time(NULL));
  int aleatory_number = rand() % ALEATORY + 1;

  // PERCORRRER O ARRAY ATÉ ENCONTRAR UMA POSIÇÃO VAZIA, CASO CONTRÁRIO, AGUARDAR VAGAR UMA POSIÇÃO
  for(int i = 0; i < size; i++){
    if(buffer[i] == 0){//ENCONTROU UM LUGAR VAZIO
      buffer[i] = aleatory_number;
      return 1;
    }else{//BUFFER CHEIO
      return -1;
    }
  }
}

void start_buffer(int *buffer){
  for(int i = 0; i < SIZE; i++){
    buffer[i] = 0;
  }
}

int main(int Np, char *args[]){
  cout << "Produtor Consumidor" << endl;
  // ================== INICIALIZANDO O BUFFER =================
  int *buffer = NULL;
  buffer = new int[SIZE];
  start_buffer(buffer);
  for(int i = 0; i < SIZE; i++){
    cout << buffer[i] << " ";
  }
  produtor(buffer, SIZE);
  consumidor(buffer, SIZE);

  // Inicializa as threads
  // pthread_t thread;
  // int erro;
  // int id_thread, t;
  //
  // for(t = 0; t < NUM; t++){
  //   printf("Criando thread %d\n", t);
  //   if(pthread_create(&thread, NULL, print, (void *)t) != 0){
  //     printf("Erro número %d\n", erro);
  //     exit(-1);
  //   }
  // }
  // pthread_exit(NULL);
  // ================ LIBERANDO A MEMÓRIA UTILIZADA NO BUFFER ==============
  delete [] buffer;
  buffer = NULL;
  return 0;
}
