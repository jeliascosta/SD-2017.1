#include <iostream>
// #include <cstdlib>
// #include <cstdio>
// #include <sys/types.h>
// #include <unistd.h>
// #include <time.h>
// #include <sstream>
// #include <iomanip>
// #include <cstring>
// #include <cmath>

#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define IN 0
#define OUT 1
#define intFIX 20

using namespace std;

// string intFixStr(int n){ //provavelmente existe uma forma MAIS RÁPIDA (mais baixo nível) do que ss, em C.
//     stringstream ss;
//     string str;
//     ss << setw(intFIX) << setfill('0') << n;
//     str = ss.str();
//     return str;
// }

int main(int argc , char *argv[])
{
    int sock, numbers;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected");

    // Mantém a comunicação com o servidor
    while(1)
    {
        printf("Enivar números para o servidor ");
        scanf("%s" , message);
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }else{
          memset(message, 0, sizeof message);
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Resposta do servidor :");
        puts(server_reply);
        memset(server_reply, 0, sizeof server_reply);
      // usleep(1000);
    }

    close(sock);
    return 0;
}
