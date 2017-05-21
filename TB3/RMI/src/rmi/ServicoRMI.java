package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.ArrayList;

public interface ServicoRMI extends Remote {
    String sayHello() throws RemoteException;
    double[] logaritmo(double[] vec, int b) throws RemoteException; // Aplicar a função logaritmo na base b a cada elemento do vetor, para b > 1.
    double[] potencia(double[] vec, int a) throws RemoteException; //  Elevar cada elemento do vetor a uma potência a > 0.
    double[] seno(double[] vec) throws RemoteException; // Aplica a função seno a cada elemento do vetor;
    double media(double[] vec, int globalN) throws RemoteException; //Calcular a média aritmética dos valores do vetor.
    Integer numMaior(double[] vec, int t) throws RemoteException; // Calcular o número de elementos do vetor que são maiores do que um valor t.
    Integer numPrimo(double[] vec) throws RemoteException; // Calcular o número de elementos do vetor que são primos.
}