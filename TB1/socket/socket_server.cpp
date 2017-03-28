#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>    //strlen
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <cmath>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#define IN 0
#define OUT 1
#define intFIX 20

using namespace std;


//Função para checar se o número é primo
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
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000], resposta[2000];

    //Criar socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Socket não pode ser criado");
    }
    puts("Socket Criado");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Aceitar conexões
    puts("Aguardando conexões...");
    c = sizeof(struct sockaddr_in);

    //Aceitar conexão de um determinado cliente
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Erro ao aceitar cliente");
        return 1;
    }
    puts("Conexão feita");

    //Recebe mensagem de um cliente
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        //Envia uma mensagem de volta ao cliente
        write(client_sock , client_message , strlen(client_message));
        puts("Recebido do cliente: ");
        puts(client_message);
        memset(client_message, 0, sizeof client_message); //limpa o buffer de entrada
    }
    if(read_size == 0)
    {
        puts("Cliente desconectado...");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("Falha ao receber dado");
    }

    return 0;
}
