/* 
 * File:   main.cpp
 * Author: Jônathan Elias Sousa da Costa
 *
 * Created on 18 de Março de 2017, 19:38
 */

#include <iostream>
#include <csignal>
#include <string>

using namespace std;

int uso(){
    cout << "USO: signalsender PID SIGNAL\n" << endl;
    cout << "PID -> número do processo (atribuído pelo SO) que receberá o sinal" << endl;
    cout << "SIGNAL -> sinal a ser enviado para o processo, dentre os disponíveis:" << endl;
    cout << "\t\tSIGUSR1, SIGUSR2 e SIGTERM" << endl;
    return EXIT_FAILURE; //valor de retorno para main deve ser código de erro (>0), indicando mal funcionamento do código do processo
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    if (argc != 3) return uso(); //para operar, precisamos do PID receptor e do sinal a ser enviado
    
    string argSinal(argv[2]); //para facilitar comparação de strings
    
    int sinal=0; //definindo sinal padrão como 0 (não causa danos a outros processos)
    
    //linhas abaixo validam argumentos de entrada
    if (argSinal == "SIGUSR1") {
        sinal = SIGUSR1;
        cout << "Sinal a ser enviado: SIGUSR1 (" << sinal << "), ";
    }
    if (argSinal == "SIGUSR2") {
        sinal = SIGUSR2;
        cout << "Sinal a ser enviado: SIGUSR2 (" << sinal << "), ";
    }
    if (argSinal == "SIGTERM") {
        sinal = SIGTERM;
        cout << "Sinal a ser enviado: SIGTERM (" << sinal << "), ";
    }
    if (sinal == 0) return uso();
    
    cout << "para o processo de PID " << atoi(argv[1]) << endl;
    
    //kill retorna -1 em caso de falha
    if (kill(atoi(argv[1]),sinal) < 0){
        cout << "Sinal não pôde ser enviado!" << endl;
        cerr << "Processo de PID " << argv[1] << " não existe." << endl;
    }
    else cout << "Sinal enviado com sucesso!" << endl;
    
    return 0;
}