/*
 * File:   main.cpp
 * Author: Jônathan Elias Sousa da Costa
 *
 * Created on 18 de Março de 2017, 19:38
 */

#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;

void signalsHandler (int signum) {
    cout << "Sinal " << signum << " recebido: ";
    if (signum == SIGUSR1) cout << "SIGUSR1.\n" << endl;
    if (signum == SIGUSR2) cout << "SIGUSR2.\n" << endl;
    if (signum == SIGTERM){
        cout << "SIGTERM (encerrar).\n" << endl;
	/* SIGTERM, por padrão, encerra o processo. Logo, basta reatribuir hanlder
         *  padrão para este sinal e dispará-lo para o próprio processo -> raise(), a fim de encerrá-lo.*/
  	signal (signum, SIG_DFL);
        raise (signum);
    }
}

int uso(){
    cout << "USO: signalreceiver WAITING_TYPE\n" << endl;
    cout << "WAITING_TYPE -> especifica a forma como o processo aguardará pelos sinais:" << endl;
    cout << "\t\tBUSY ou BLOCK" << endl;
    return EXIT_FAILURE; //valor de retorno para main deve ser código de erro (>0), indicando mal funcionamento do código do processo
}


/*
 *
 */
int main(int argc, char** argv) {
    
    if (argc != 2) return uso(); //para operar, precisamos do tipo de waiting
    
    int startDone=0; //variável de controle para acompanhar criação dos handlers
    
    cout << "O PID deste processo --> " << getpid() << "\n" << endl; //precisamos do PID deste processo, para o signalSender
    
    //signal() retorna 0 caso consiga associar manipulador ao sinal
    if (!(signal (SIGUSR1, signalsHandler))){
        cout << "Signal handler para SIGUSR1 criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGUSR1." << endl;
    
    if (!(signal (SIGUSR2, signalsHandler))){
        cout << "Signal handler para SIGUSR2 criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGUSR2." << endl;
    
    if (!(signal (SIGTERM, signalsHandler))){
        cout << "Signal handler para SIGTERM criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGTERM." << endl;

    if (startDone == 3) cout << "Handlers criados com sucesso!\n" << endl;
    else cerr << "Falha na criação dos handlers." << endl;

    string waitType(argv[1]);
    
    if (waitType == "BUSY"){
        cout << "Esperando por sinais em modo busy.\n" << endl;
        while (true); //"busy waiting" pelos sinais. Processo está sempre em estado running,
                      //                             e é interrompido quanto um sinal chega.
    }
    else {
        if (waitType == "BLOCK"){
            cout << "Esperando por sinais em modo blocking.\n" << endl;
            while (true) sleep(1); //blocking wait pelos sinais. Processo está sempre em estado waiting
                                   //baseado em timer de 1s, e é "despertado" quando o timer expira ou o sinal chega.
        }
        else return uso();
    }
    //as implementações dos tipos de waiting podem ser confirmados pela observação do estado do processo no gerenciador de tarefas do SO.

    cout << "Encerrado." << endl; //esta linha não será executada neste programa, dado o loop infinito de waiting.
                                  //SIGTERM encerra o processo diretamente após a execução do handler.
    return 0;
}
