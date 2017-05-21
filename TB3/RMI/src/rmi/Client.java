package rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Random;

public class Client {
    public static final int N = 100000000;
    //public static final int N = 10000000; //Lembar de colocar mais um zero
    
    public static double[] globalVector = new double[N];
    
    private static double globalResult = 0;
    
    public static final int randomBound = 1000;
    
    public static synchronized void addToGlobalResult(double localResult){
        globalResult+=localResult;        
    }
    
    public static ServicoRMI servicoRMI;
    
    private Client() {}
    
    public static void uso(){
        System.err.println("USO: client.sh [número de threads] [funcao].\n");
        System.err.println("funcao: LOGARITMO POTENCIA SENO MEDIA CONTAMAIORQUE CONTAINTERVALO.\n");
        System.exit(1);        
    }
    public static void main(String[] args) {
        //String host = (args.length < 1) ? null : args[0]; //Primeiro argumento da cmd será o endereço do host remoto (Server)
        if (args.length < 2) uso();
        Integer K = (args.length < 1) ? 4 : (new Integer(args[0])); //Primeiro argumento da cmd será o número de thrads
        String funcao = args[1];
        
        try {
            Registry registroRMI = LocateRegistry.getRegistry(); //Procura servidor de registros RMI no host indicado (null = local)
            servicoRMI = (ServicoRMI) registroRMI.lookup("ServicoRMI"); //Solicita acesso ao servico RMI com o nome indicado cadastrado no registro
            
            Random rand = new Random(); //http://stackoverflow.com/questions/4307273/how-can-i-create-and-display-an-arraylist-of-random-numbers-in-java
            rand.setSeed(System.currentTimeMillis());            
            
            for (int i = 0; i < N ; i++)
                globalVector[i] =  rand.nextInt(randomBound); //Trabalharemos com potenciação de expoente 2 no servidor. Isso explica este Upper Bound
            
            System.out.print("1º el. pré RMI: "+globalVector[0]+";");
            System.out.print("Nº el. pré RMI: "+globalVector[N-1]+";");  
           
            long startTime = System.nanoTime();
            ArrayList<Thread> threads = new ArrayList();
            boolean transformaVetor = false;
            int chunckSize = N/K;
            for (int i=0;i<K;i++){
                int start=i*chunckSize;
                int end = (i+1)*chunckSize;
                end = ((i+1) == K)?(N):(end);
                switch(funcao){
                  case "LOGARITMO": {
                        threads.add(new LogartimoThread(i, start, end));
                        transformaVetor = true;
                        break;
                  }
                  case "POTENCIA": {
                        threads.add(new PotenciaThread(i, start, end));
                        transformaVetor = true;
                        break;
                  }
                  case "SENO": {
                        threads.add(new SenoThread(i, start, end));
                        transformaVetor = true;
                        break;
                  }
                  case "MEDIA": {
                        threads.add(new MediaThread(i, start, end));
                        transformaVetor = false;
                        break;
                  }
                  case "CONTAMAIORQUE": {
                        threads.add(new ContaMaiorQueThread(i, start, end));
                        transformaVetor = false;
                        break;
                  }
                  case "CONTAINTERVALO": {
                        threads.add(new ContaIntervaloThread(i, start, end));
                        transformaVetor = false;
                        break;
                  }
                  default: System.exit(1);
                  }
            }
            
            for(int i=0;i<threads.size();i++) {
                threads.get(i).start();
            }
            
            for(int i=0;i<threads.size();i++) {
                threads.get(i).join();
            }
            
            if(transformaVetor) {
                System.out.print("1º el. pós RMI: "+globalVector[0]+";");
                System.out.print("Nº el. pós RMI: "+globalVector[N-1]+";");
            }
            else {
                System.out.print("Resultado RMI: "+globalResult+";");
            }
            System.out.print(threads.size()+" Threads;");
            System.out.print("Tempo de execução: ;"+(System.nanoTime()-startTime));

            
        } catch (Exception e) {
            System.err.println("Exceção no Cliente: " + e.toString());
            e.printStackTrace();
        }
        System.out.println();
    }
}