package rmi;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;


public class Server implements ServicoRMI {
    
    public Server() {} //Construtor do servidor precisa ser público?

    @Override
    public String sayHello() throws RemoteException {
        return "Hello, world!";
    }

    @Override
    public double[] logaritmo(double[] vec, int b) throws RemoteException {
        double[] vec_log = vec; 
        System.out.println("Server vecSize: "+vec_log.length);
        for (int i=0;i<vec_log.length;i++) 
            vec_log[i] = Math.log(vec_log[i])/Math.log(b); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec_log;
    }

    @Override
    public double[] potencia(double[] vec, int a) throws RemoteException {
        double[] vec_log = vec;
        System.out.println("Server vecSize: "+vec_log.length);
        for (int i=0;i<vec_log.length;i++) 
            vec_log[i] = Math.pow(vec_log[i], a); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec_log;
    }

    @Override
    public double[] seno(double[] vec) throws RemoteException {
        double[] vec_log = vec;
        System.out.println("Server vecSize: "+vec_log.length);
        for (int i=0;i<vec_log.length;i++) 
            vec_log[i] = Math.sin(vec_log[i]); //Propriedade matemática: log de a na base b = (ln de a)/(ln de b)
        return vec_log;   
    }

    @Override
    public double media(double[] vec, int globalN) throws RemoteException {
        double soma = 0;
        for (int i=0;i<vec.length;i++)
            soma += vec[i];
        return soma/globalN;
    }

    @Override
    public Integer numMaior(double[] vec, int t) throws RemoteException {
        return Integer.MAX_VALUE;
    }

    @Override
    public Integer numPrimo(double[] vec) throws RemoteException {
        return Integer.MIN_VALUE;
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
