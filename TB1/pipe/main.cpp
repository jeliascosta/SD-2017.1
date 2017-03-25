/*
 * File:   main.cpp
 * Author: Jônathan Elias Sousa da Costa
 *
 * Created on 19 de Março de 2017, 18:03
 */

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include <cstring>

#define IN 0
#define OUT 1
#define intFIX 20

using namespace std;

//Filho lê números do pipe e imprime
void printPipe (int pipeFile) {
  FILE *pipeStream = fdopen (pipeFile, "r");

  //bloco abaixo implementa dica do professor no enunciado.  
  char str[intFIX+1];
  for (int i=0;i<intFIX-1;i++) str[i]='0';
  str[intFIX-1]='1';
  str[intFIX]='\0';
  
  char zero[intFIX+1];
  for (int i=0;i<intFIX;i++) zero[i]='0';
  zero[intFIX]='\0';

  while (strcmp(str,zero) != 0){
    fread(str,1,intFIX,pipeStream);
    str[intFIX]='\0';puts(str);
  }
  cout << "ACHEI O ZERO!" << endl;
  fclose (pipeStream);
}

string intFixStr(int n){ //provavelmente existe uma forma MAIS RÁPIDA do que ss, em C.
    stringstream ss;
    string str;
    ss << setw(intFIX) << setfill('0') << n;
    str = ss.str();
    return str;
}

//Pai escreve números aleatórios crescentes no pipe
void randomToPipe (int pipeFile, int n) {
    FILE *pipeStream = fdopen (pipeFile, "w");

    int lastRand = 0, currRand = -1;

    srand(time(NULL)); //Inicializar biblioteca random com semente única à instância do processo

    int i, j;
    int delta = RAND_MAX / n;
    string str;
    //Bolei uma forma de obter aleatórios crescentes, como pedido no enunciado.
    //Me baseei nos seguintes requisitos:
    //1: impedir o algoritmo de obter RAND_MAX nas primeiras n-1 iterações
    //2: garantir a finitude do loop
    //A cada iteração o loop é obrigado a quebrar somente quando o número retornado é maior que o anterior.
    //O intervalo de números a escolher varia sempre do número atual até o delta mais próximo.
    //Isso é garantido ao tomar o módulo j (delta). 
    for (i = 0, j = delta; i < n; i++, j += delta){
        while(currRand <= lastRand) currRand = rand() % j;
        lastRand = currRand;
	fprintf(pipeStream, "%s",intFixStr(currRand).c_str());
    }
    str = intFixStr(0);
    fprintf(pipeStream, "%s, ", str.c_str());
    fclose (pipeStream);
}

int uso(){
    cout << "USO: pipe NCOUNT\n" << endl;
    cout << "\tNCOUNT -> número (>0) de números a serem gerados pelo processo produtor.\n" << endl;
    return EXIT_FAILURE; //valor de retorno para main deve ser código de erro (>0), indicando mal funcionamento do código do processo
}

int main (int argc, char** argv) {
  if (argc != 2) return uso(); //Para operar, precisamos do número de números para o processo produtor.
  if (atoi(argv[1]) == 0) return uso();

  pid_t pid; //pid_t é o tipo de dado específico ao SO ao lidar com ids de processos.
  int Pipe[2]; //O pipe linux é implementado através de um vetor com duas posições: 0 -> stdin; 1 -> stdout.
               //Visível a dois processos: pai e filho; ou irmãos.

  if (pipe(Pipe)){ //Função pipe() instancia um pipe no vetor Pipe[]. Retorna 1 em caso de falha.
      cerr << "Falha no instanciamento de um pipe. Encerrando ...\n" << endl;
      return EXIT_FAILURE;
  }
  else cout << "Pipe instanciado com sucesso!" << endl;

  pid = fork(); //Função (system call) fork() cria novo processo no SO, cópia do processo corrente.
                //Em caso de sucesso, retorna, ao processo pai, o PID atribuído ao processo filho; retorna 0 ao processo filho.
                //Caso contrário, retorna -1 ao processo pai.

//Próximo bloco de instruções será executado por AMBOS os processos.
   if (pid == (pid_t) 0){ //Se pid=0 estamos no processo filho.
      //Fechar ponta OUT do pipe. Processo filho só lê.
      close (Pipe[OUT]);
      printPipe(Pipe[IN]);
      cout<<"Encerrou Filho..."<<endl;
      return EXIT_SUCCESS;
   }
  else if (pid < (pid_t) 0){ //Se processo pai obtêm retorno negativo de fork(), system call falhou no objetivo.
      cerr << "Falha na criação do Fork.\n" << endl;
      return EXIT_FAILURE;
    }
  else {
      //Fork foi bem-sucedido e estamos no processo pai, que só escreve no pipe, então fechamos ponta IN.
      close(Pipe[IN]);
      randomToPipe(Pipe[OUT], atoi(argv[1]));
      cout<<"Encerrou Pai..."<<endl;
      return EXIT_SUCCESS;
  }
}
