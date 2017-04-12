/*
 * File:   main.cpp
 * Author: Jônathan Elias Sousa da Costa
 *
 * Created on 11 de Abril de 2017, 22:20
 */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <chrono> //C++11
#include <cstdint>

//Requisito funcional: teto de número aleatório é 10⁷.
#define CEIL 10000001
//Requisito funcional: processo encerra ao consumir 10⁴ números.
#define M 1000

using namespace std;

int N=1000;
int Np = 20, Nc = 20;

int *buffers;
volatile int consumeCount=0;
    
sem_t empty; //Semáforo contador que controla extração de dados na memória compartilhada.
sem_t full; //Semáforo contador que controla inserção de dados na memória compartilhada.
sem_t mutex; //Semáforo mutex que implementa região crítica na inserção/extração de dados dos buffers. Também poderia ser pthread_mutex_t.
pthread_mutex_t randMutex; //Semáforo mutex para implementar região exclusiva nos produtores ao gerarem números aleatórios com rand(), que não é thread-safe.
pthread_mutex_t countMutex; //Semáforo mutex para implementar região exclusiva nos consumidores ao compararem contador de consumo com M.

int getEmptyBuffer(){
    for (int i=0;i<N;i++) if (buffers[i]==0) {/*cerr << "getEmpty retornando " << i << endl;*/return i;}
    cerr << "Produtor acionado sem nenhum buffer livre na memória compartilhada! Falha nos semáforos!" << endl;
    exit(1);
}

int getFullBuffer(){
    for (int i=0;i<N;i++) if (buffers[i]!=0) return i;
    cerr << "Consumidor acionado sem nenhum buffer cheio na memória compartilhada! Falha nos semáforos!" << endl;
    exit(1);
}

bool checkPrime(long int li){
    if (li < 1) return false;
    if (li == 1 || li == 2 || li == 3) return true;
    long int root = sqrt(li), i = 1;
    bool isPrime = true;
    //Se, para um número real > 1 não encontarmos divisores maiores que 1 e menores que sua raiz, tal número é primo.
    while (( ++i < root) && isPrime) if ((li % i) == 0) isPrime = false;
    return isPrime;
}

void* producer(void *id){
    int ID = (intptr_t)id; //intptr_t é definido em cstdint e é a "forma correta" de se reinterpretar um ponteiro para int em C++ 64 bits.
    while (1){
            pthread_mutex_lock(&randMutex); //rand() não é thread-safe. Se múltiplas threads chamarem, como é o caso, precisamos operar em região exclusiva. srandr() foi executado uma vez na main().
            //cout << "Olá, eu sou o produtor " << ID << endl;
                int random = rand() % CEIL;
                //cout << "P"<<ID<< " random " << random << endl;
            pthread_mutex_unlock(&randMutex);        
            //cout << "P" << ID << ": Meu aleatório é " << random << endl;

            sem_wait(&empty); //Com conteúdo já produzido, produtor solicita a semáforo que controla buffers vazios "liberar passagem". Se nenhum buffer estiver vazio -> empty=0 -> consumidores ainda não operaram -> este produtor bloqueia.
                sem_wait(&mutex);
                    int pos = getEmptyBuffer();
                    buffers[pos] = random;
                    //cout << "P"<<ID<< " buffer " << buffers[pos] << endl;
                sem_post(&mutex);
            sem_post(&full); //Um produtor encerra incrementando semáforo que controla buffers cheios. Aciona a operação de um consumidor bloqueado.
    }   
    //if (random > CEIL) cout << "ERRO" << endl;
}

void* consumer(void *id){
    int ID = (intptr_t)id; //intptr_t é definido em cstdint e é a "forma correta" de se reinterpretar um ponteiro para int em C++ 64 bits.
    int localCount=0;
    
    int n=0;
    while(localCount++ < M){
        sem_wait(&full); //Um consumidor (re)inicia aguardando semáforo que controla buffers cheios "liberar passagem". Se nenhum buffer estiver cheio -> full=0 -> produtores ainda não operaram -> este consumidor bloqueia.
            sem_wait(&mutex);
                    int pos = getFullBuffer();
                    n = buffers[pos];
                    buffers[pos] = 0;
                    consumeCount++;
            sem_post(&mutex);
        sem_post(&empty); //Um consumidor encerra incrementando semáforo que controla buffers vazios. Aciona a operação de um produtor bloqueado.

        printf("Olá, eu sou o consumidor %d\nEstou consumindo o número: %d,  É primo? %d\nContagem de consumo: %d\n", ID, n, checkPrime(n), consumeCount);
    }
    printf("\nC%d encerra. lC %d gC %d.\n",ID,localCount,consumeCount);
}

int use(){
    cout << "USO: semaphores N Np Nc\n" << endl;
    cout << "N -> O número de números a serem produzidos/consumidos (tamanho da memória compartilhada)." << endl;;
    cout << "Np -> O número de threads do tipo produtor." << endl;;
    cout << "Nc -> O número de threads do tipo consumidor." << endl;
    return EXIT_FAILURE; //valor de retorno para main deve ser código de erro (>0), indicando mal funcionamento do código do processo
}


int main(int argc, char** argv) {
    //if (argc != 4) return use(); //Para operar, precisamos do número de threads do tipo produtor e consumidor, aleḿ do número de números
    
    /*N = atoi(argv[1]);
    Np = atoi(argv[2]);
    Nc = atoi(argv[3]);*/
 
    buffers = (int *)malloc(N * sizeof(int)); //Alocando dinamicamente memória compartilhada: não sabemos tamanho de N à priori.     
    if (buffers == NULL){
        cerr << "Falha ao alocar memória compartilhada!" << endl;
        return EXIT_FAILURE;
    }
    
    srand(time(NULL)); //Inicializar biblioteca random com semente única à instância do processo
    
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start); //Solução POSIX C++98 para medição precisa de tempo entre dois pontos. REF: https://www.softwariness.com/articles/monotonic-clocks-windows-and-posix/
    
    auto begin = chrono::high_resolution_clock::now(); //Solução C++11 para medição precisa de tempo entre dois pontos. REF: http://www.cplusplus.com/reference/chrono/high_resolution_clock/now/

        //Início do trecho de código a ter o tempo mensurado
        for (int i=0;i<N;i++) buffers[i] = 0;
        /*for (int i=0;i<N;i++) {
            cout << buffers[i] << " ";
            cout << checkPrime(buffers[i]) << endl;
        }*/
        sem_init(&empty, 0, N); //Inicializando semáforo empty com contagem = tamanho da memória compartilhada. De início, todos os buffers estão vazios.
        sem_init(&full, 0, 0); //Inicializando semáforo full com contagem zerada. De início, nenhum buffer está cheio. 
        sem_init(&mutex, 0, 1); //Inicializando semáforo mutex que controla região crítica de inserção/extração no buffer com contagem em 1. A contagem alternará entre 1 e 0 (tipo lock);
        pthread_mutex_init(&countMutex, 0); //Utilizaremos para serializar acesso à contagem de consumo, que encerrará processo.
        pthread_mutex_init(&randMutex, 0); //Utilizaremos 0 como flag "buffer livre" para todas as posições
        pthread_t Producers[Np];
        pthread_t Consumers[Nc];
        for(int i=0;i<Np;i++)
            pthread_create(&Producers[i], NULL, producer, (void *)i);        
        for(int i=0;i<Nc;i++)
            pthread_create(&Consumers[i], NULL, consumer, (void *)i);
        for(int i=0;i<Np;i++)
            pthread_join(Producers[i], NULL);
        for(int i=0;i<Nc;i++){
            pthread_join(Consumers[i], NULL);
        }
        pthread_mutex_destroy(&randMutex);
        //Final do trecho de código a ter o tempo mensurado

    auto end = chrono::high_resolution_clock::now();

    clock_gettime(CLOCK_MONOTONIC, &finish);

    cout.flush(); //Após impressão massiva na tela buffer pode estar grande -> liberar possíveis resíduos antes de próximas impressões.
    cout << "\n\nTempo de execução pela C++11 chrono:\t" << chrono::duration_cast<chrono::nanoseconds>(end-begin).count() << "ns" << endl;

    elapsed = ((finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0)*(1000000000);
    cout << "\n\nTempo de execução pela POSIX time:\t" << elapsed << "ns" << endl;
    
    //for (int i =0;i<N;i++) cout << i << " " << buffers[i] << endl;
    delete [] buffers; //Não esquecer de desalocar memória compartilhada dinâmica.

    return 0;
}

