package rmi;

import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.logging.Level;
import java.util.logging.Logger;

public class ContaMaiorQueThread extends Thread {
    private final int nroThread;
    private final int start;
    private final int end;
    private double[] localVector;
    private double localResult;
    
    public ContaMaiorQueThread(int nroThread, int start, int end){
        this.nroThread = nroThread;
        this.start = start;
        this.end = end;
        localVector = new double[end-start];
    }
    
    @Override
    public void run(){
        System.arraycopy(Client.globalVector,start,localVector,0,end-start); //Copiar parte de globalVector alocada para esta thread
       
        try { localResult = Client.servicoRMI.contaMaiorQue(localVector,Client.randomBound/2); } //Passar vetor para método RMI logaritmo
        catch (RemoteException ex) { Logger.getLogger(ContaIntervaloThread.class.getName()).log(Level.SEVERE, null, ex); }

        Client.addToGlobalResult(localResult); //Guardar em globalVector processamento alocado a esta thread
    }
}
