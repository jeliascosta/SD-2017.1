#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctime>


using namespace std;

#define NUM 10
#define Np 2
#define Nc 10
#define BUFFER 100
#define ALEATORY 100

void *consumidor(void *buffer){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada
  // PERCORRE O VETOR EM BUSCA DE UMA POSIÇÃO DIFERENTE DE 0
  int *buff = (int *)buffer;
  for(int i = 0; i < BUFFER; i++){
    if(buff[i] != 0){//VERIFICA SE O NÚMERO É PRIMO E RETORNA
      cout << buff[i] << endl;
      buff[i] = 0;
      break;
    }
  }
}

void *produtor(void *buffer){//Recebe como parâmetro uma referência para a thread e para a memória compartilhada
  // GERAR NÚMEROS ALEATÓRIOS
  int *buff = (int *)buffer;

  // PERCORRRER O ARRAY ATÉ ENCONTRAR UMA POSIÇÃO VAZIA, CASO CONTRÁRIO, AGUARDAR VAGAR UMA POSIÇÃO
  for(int i = 0; i < BUFFER; i++){
    if(buff[i] == 0){//ENCONTROU UM LUGAR VAZIO
      // srand(time(NULL));
      int aleatory_number = rand() % ALEATORY + 1;
      buff[i] = aleatory_number;
      break;
    }
  }
}

void start_buffer(int *buffer){
  for(int i = 0; i < BUFFER; i++){
    buffer[i] = 0;
  }
}

int main(int argc, char *args[]){
  cout << "Produtor Consumidor" << endl;
  // ================== INICIALIZANDO O BUFFER =================
  int *buffer = NULL; //Cria um ponteiro para inteiro
  buffer = new int[BUFFER]; //Inicializa um vetor de inteiros
  start_buffer(buffer); // Preenhce com zeros
  // produtor(buffer, SIZE);
  // consumidor(buffer, SIZE);
  // ================= Inicializa as threads ========================
  pthread_t thread[Np];
  // ==================== THREADS PRODUTORAS ======================
  for(int t = 0; t < Np; t++){
    if(pthread_create(&thread[t], NULL, produtor, (void *)buffer) != 0){
      break;
    }
    // cout << "thread " << t << " criada" << endl;
  }

  for(int i = 0; i < Np; i++)
    pthread_join(thread[i], NULL);

  cout << endl;
  for(int i = 0; i < BUFFER; i++){

    cout << buffer[i] << " ";
  }
  cout << endl;

  // ==================== THREADS CONSUMIDORAS ======================
  // for(int t = 0; t < Nc; t++){
  //   if(pthread_create(&thread, NULL, consumidor, (void *)(intptr_t)buffer) != 0){
  //     exit(-1);
  //   }
  // }

  pthread_exit(NULL);
  // ================ LIBERANDO A MEMÓRIA UTILIZADA NO BUFFER ==============
  delete [] buffer;
  buffer = NULL;
  return 0;
}
