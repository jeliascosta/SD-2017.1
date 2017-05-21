package rmi;

import java.rmi.RemoteException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MediaThread extends Thread {
    private final int nroThread;
    private final int start;
    private final int end;
    private double[] localVector;
    private double localResult;
    
    public MediaThread(int nroThread, int start, int end){
        this.nroThread = nroThread;
        this.start = start;
        this.end = end;
        localVector = new double[end-start];
    }
    
    @Override
    public void run(){
        System.out.println("\n Rodou MeuThread nro: "+this.nroThread+"! Start Offset: "+this.start+" , End Offset: "+this.end+", localVec size:"+localVector.length);
        
        int k = 0;
        for (int i = start; i < end; i++)
            localVector[k++] = Client.globalVector[i]; //Copiar parte de globalVector alocada para esta thread
        
        /*for (int i=0; i < localVector.length; i++)
            System.out.println(nroThread + " antes de log() "+i+" - "+localVector[i]);*/
        
        try { localResult = Client.servicoRMI.media(localVector, 2); } //Passar vetor para método RMI logaritmo
        catch (RemoteException ex) { Logger.getLogger(LogartimoThread.class.getName()).log(Level.SEVERE, null, ex); }
        
        /*for (int i=0;i<localVector.length;i++)
            System.out.println(nroThread + " após log() "+i+" - "+localVector[i]);*/
        
        k=0;
        for (int i = start; i < end; i++)
            Client.globalVector[i] = localVector[k++]; //Guardar em globalVector processamento alocado a esta thread
    }
}
