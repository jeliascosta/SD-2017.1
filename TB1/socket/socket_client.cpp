#include <iostream>
#include <sstream>
#include <iomanip>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <math.h>
#include <stdlib.h>

#define IN 0
#define OUT 1
#define intFIX 20

using namespace std;

bool checkPrime(long int li){
    if (li < 1) return false;
    if (li == 1 || li == 2 || li == 3) return true;
    long int root = sqrt(li), i = 1;
    bool isPrime = true;
    //Se, para um número real > 1 não encontarmos divisores maiores que 1 e menores que sua raiz, tal número é primo.
    while (( ++i < root) && isPrime) if ((li % i) == 0) isPrime = false;
    return isPrime;
}

int main(int argc , char *argv[])
{
    int sock, numbers;
    struct sockaddr_in server;
    char message[intFIX+1] , server_reply[intFIX+1]; //intFIX posições + '\0' ("terminação" de string)

    //Create socket
    sock = socket(PF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Erro ao criar socket");
    }
    puts("Socket criado");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = PF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Falha ao conectar. Error");
        return 1;
    }

    puts("Conectado!");

    long int li = -1;

    // Mantém a comunicação com o servidor
    while(li != 0)
    {
        //Receive a reply from the server
        //O consumidor se conecta ao produtor e aguarda a chegada de um número
        printf("Aguardando número (string) do servidor produtor...\n ");
        if( recv(sock , server_reply , intFIX+1 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

		    server_reply[intFIX] = '\0'; //garantindo término de string
        li = strtol(server_reply,NULL,10); //convertendo de string para long int

        printf("String recebida: %s, que equivale ao número %ld\n",server_reply,li);
        memset(server_reply, 0, sizeof server_reply);

        //Send some data
        string ehPrimo;
        if (checkPrime(li)) ehPrimo = "O número É PRIMO.";
        else ehPrimo = "O número NÃO É primo.";

        if( send(sock , ehPrimo.c_str() , ehPrimo.length() , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }else{
         printf("Resultado do teste de primalidade já foi enviado servidor produtor...\n\n ");
          memset(message, 0, sizeof message);
        }

      // usleep(1000);
    }
puts("ACHEI O ZERO");
    close(sock);
    return 0;
}
