package rmi;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;


public class Server implements ServicoRMI {
    
    private Server() {} //Construtor do servidor precisa ser público?

    @Override
    public String sayHello() throws RemoteException {
        return "Hello, world!";
    }

    @Override
    public double[] logaritmo(double[] vec, int b) throws RemoteException {
        for (int i=0;i<vec.length;i++) 
            vec[i] = Math.log(vec[i])/Math.log(b); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec;
    }

    @Override
    public double[] potencia(double[] vec, int a) throws RemoteException {
        for (int i=0;i<vec.length;i++) 
            vec[i] = Math.pow(vec[i], a); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec;
    }

    @Override
    public double[] seno(double[] vec) throws RemoteException {
        for (int i=0;i<vec.length;i++)
            vec[i] = Math.sin(vec[i]); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec;   
    }

    @Override
    public double media(double[] vec, int globalN) throws RemoteException {
        double soma = 0;
        for (int i=0;i<vec.length;i++){
            soma += vec[i]/globalN;
        }
        return soma;
    }

    @Override
    public int contaMaiorQue(double[] vec, double t) throws RemoteException {
        int contagem=0;
        for (int i=0;i<vec.length;i++){
            if (vec[i] > t) contagem++; 
        }
        return contagem;
    }

    @Override
    public int contaIntervalo(double[] vec, double n, double dist) throws RemoteException {
        int contagem=0;
        for (int i=0;i<vec.length;i++){
            if (Math.abs(vec[i] - n) <= Math.abs(dist)) contagem++;
        }
        return contagem;
    }
    
    
    public static void main(String args[]) {        
        try {
            Server obj = new Server();
            ServicoRMI stub = (ServicoRMI) UnicastRemoteObject.exportObject(obj, 0);

            // Bind the remote object's stub in the registry
            Registry registry = LocateRegistry.getRegistry();
            registry.bind("ServicoRMI", stub);

            System.err.println("Servidor pronto");
        } catch (Exception e) {
            System.err.println("Exceção no servidor: " + e.toString());
            e.printStackTrace();
        }
    }
}
