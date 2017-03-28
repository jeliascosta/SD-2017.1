#include <stdio.h>
#include <stdlib.h>
#include <string.h>    //strlen
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <netinet/in.h>
#include <unistd.h>    //write
#include <cmath>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>

#define IN 0
#define OUT 1
#define intFIX 20

using namespace std;


string intFixStr(int n){ //provavelmente existe uma forma MAIS RÁPIDA (mais baixo nível) do que ss, em C.
    stringstream ss;
    string str;
    ss << setw(intFIX) << setfill('0') << n;
    str = ss.str();
    return str;
}

int main(int argc , char *argv[])
{
	if(argc!=2){
		puts("Digite a quantidade de números aleatórios a serem gerados!");
		return 1;
	}

    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000], resposta[2000];

    //Criar socket
    socket_desc = socket(PF_INET , SOCK_STREAM , 0);
    if (socket_desc < 0){
        printf("Socket não pode ser criado");
    }
    puts("Socket Criado");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons( 8888 );

    //Bind
    if(::bind(socket_desc,(struct sockaddr *) &server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        // return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Aceitar conexões
    puts("Aguardando conexões...");
    c = sizeof(struct sockaddr_in);

    //Aceitar conexão de um determinado cliente, ou seja, do consumidor
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Erro ao aceitar cliente");
        return 1;
    }
    puts("Conexão feita");


    int lastRand = 0, currRand = -1;

    srand(time(NULL)); //Inicializar biblioteca random com semente única à instância do processo

	int n =atoi(argv[1]); //precisamos do número de números aleatórios

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
		if ((write(client_sock , intFixStr(currRand).c_str() , intFIX+1) > 0))
		 puts("Número enviado!");
		else return 1;
        //Aguarda uma mensagem de volta do consumidor cliente
        read_size = recv(client_sock , client_message , 25 , 0);
        puts("Recebido do consumidor: ");
        puts(client_message);
        puts("");
        memset(client_message, 0, sizeof client_message); //limpa o buffer de entrada
    }
    str = intFixStr(0);

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
