/*
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

void termination_handler (int signum) {
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


/*
 *
 */
int main(int argc, char** argv) {
    int startDone=0; //variável de controle para acompanhar criação dos handlers
    
    cout << "O PID deste processo --> " << getpid() << endl; //precisamos do PID deste processo, para o signalSender
    
    //signal() retorna 0 caso consiga associar manipulador ao sinal
    if (!(signal (SIGUSR1, termination_handler))){
        cout << "Signal handler para SIGUSR1 criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGUSR1." << endl;
    
    if (!(signal (SIGUSR2, termination_handler))){
        cout << "Signal handler para SIGUSR2 criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGUSR2." << endl;
    
    if (!(signal (SIGTERM, termination_handler))){
        cout << "Signal handler para SIGTERM criado com sucesso!" << endl;
	startDone++;        
    }
    else cout << "Falha ao criar signal handler para SIGTERM." << endl;

    if (startDone == 3) cout << "Handlers criados com sucesso!\n" << endl;
    else cerr << "Falha na criação dos handlers." << endl;

    while(true);

    cout << "Encerrado." << endl;
    
    return 0;
}
