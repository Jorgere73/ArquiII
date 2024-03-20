public class Veiga{

    public static void main(String [] args){
        //Semaphore semaforo = new Semaphore(1);
        Meigas[] meiga = new Meigas[30];
        Maruxa maruxa = new Maruxa();
        Sinforiano sinforiano = new Sinforiano();
        Anton anton = new Anton();

        for(int i = 0; i< 30;i++)
        {
            meiga[i]=new Meigas();
            meiga[i].start();
        }

        sinforiano.start();
        maruxa.start();
        anton.start();
    

        sinforiano.Join();
        anton.estoy_vivo = false;
        anton.Join();
        maruxa.Join();

        
        for(int i = 0; i< 30;i++)
        {
            meiga[i].Join();
        }

        System.out.println("Fin de la simulacion");
        System.exit(0); 

    }

}