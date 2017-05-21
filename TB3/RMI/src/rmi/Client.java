package rmi;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Random;

public class Client {
    //private static final int N = 100000000;
    public static final int N = 10000; //Lembar de colocar mais um zero
    
    public static double[] globalVector = new double[N];
    
    public static double globalResult = 0;
    
    public static ServicoRMI servicoRMI;
    
    private Client() {}
    
    public static void uso(){
        System.err.println("USO: client.sh [número de threads] [funcao].\n");
        System.err.println("funcao: LOGARITMO POTENCIA SENO MEDIA CONTAMAIOR CONTAPRIMO.\n");
        System.exit(1);        
    }
    public static void main(String[] args) {
        //String host = (args.length < 1) ? null : args[0]; //Primeiro argumento da cmd será o endereço do host remoto (Server)
        if (args.length < 2) uso();
        Integer K = (args.length < 1) ? 4 : (new Integer(args[0])); //Primeiro argumento da cmd será o número de thrads
        String funcao = args[1];
        
        System.out.println("Operando com "+K+" threads.");
        try {
            Registry registroRMI = LocateRegistry.getRegistry(); //Procura servidor de registros RMI no host indicado (null = local)
            servicoRMI = (ServicoRMI) registroRMI.lookup("ServicoRMI"); //Solicita acesso ao servico RMI com o nome indicado cadastrado no registro
            System.out.println(Integer.BYTES+" - "+Integer.SIZE);
            Random rand = new Random();//http://stackoverflow.com/questions/4307273/how-can-i-create-and-display-an-arraylist-of-random-numbers-in-java
            rand.setSeed(System.currentTimeMillis());
            
            for (int i=0; i<N ;i++) 
                globalVector[i] =  rand.nextInt(Integer.MAX_VALUE-1);
                //globalVector[i] =  rand.nextInt(10);
      
            System.out.println(globalVector.length);
            System.out.println(globalVector[0]);
            System.out.println(globalVector[N-1]);   
            
            /*for (int i = 0; i < globalVector.length; i++)
                System.out.println(i + " - " + globalVector[i]);*/
            boolean transformaVetor = false;
            int chunckSize = N/K;
            for (int i=0;i<K;i++){
                int start=i*chunckSize;
                int end = (i+1)*chunckSize;
                end = ((i+1) == K)?(N):(end);
                switch(funcao){
                  case "LOGARITMO": {
                        new LogartimoThread(i, start, end).start();
                        transformaVetor = true;
                        break;
                  }
                  case "POTENCIA": {
                        new PotenciaThread(i, start, end).start();
                        transformaVetor = true;
                        break;
                  }
                  case "SENO": {
                        new SenoThread(i, start, end).start();
                        transformaVetor = true;
                        break;
                  }
                  case "MEDIA": {
                        new MediaThread(i, start, end).start();
                        transformaVetor = false;
                        break;
                  }
/*                  case "MEDIA": {
                        new MediaThread(i, start, end).start();
                        transformaVetor = false;
                        break;
                  }
                  case "MEDIA": {
                        new MediaThread(i, start, end).start();
                        transformaVetor = false;
                        break;
                  }*/

                  default: System.exit(1);
                  }
            }

            Thread.sleep(4000);
            
            /*for (int i = 0; i < globalVector.length; i++)
                System.out.println(i + " - " + globalVector[i]);*/
            
  
            
            if(transformaVetor) {
                System.out.println(globalVector.length);
                System.out.println(globalVector[0]);
                System.out.println(globalVector[N-1]); 
            }
            else {
                //System.out.println();
            }
            
            /*String response1 = servicoRMI.sayHello(); //Chamada remota de método
            System.out.println("resposta sayHello(): " + response1);
            Integer response3 = servicoRMI.media(globalVector); //Chamada remota de método
            System.out.println("resposta mean(): " + response3);
            Integer response4 = servicoRMI.numMaior(globalVector, 5); //Chamada remota de método
            System.out.println("resposta numBigger(): " + response4);
            Integer response5 = servicoRMI.numPrimo(globalVector); //Chamada remota de método
            System.out.println("resposta numPrime(): " + response5);
            /*double[] response6 = servicoRMI.potencia(7); //Chamada remota de método
            System.out.println("resposta pow(): " + response6[0]);
            double[] response7 = servicoRMI.seno(); //Chamada remota de método
            System.out.println("respostas sin(): " + response7[0]);*/
        } catch (Exception e) {
            System.err.println("Exceção no Cliente: " + e.toString());
            e.printStackTrace();
        }
    }
}