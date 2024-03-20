public class Meigas extends Hilos{
    private Trazador trazador;
    private static final int tab = 5;
    public Meigas()
    {
        trazador = new Trazador(tab, "Meigas");
    }
    
      public void run()
    {
        
        for(int i = 0;i < 5;i++)
        {
            trazador.Print("hola soy meiga");
            Hilos.Pausa(1000);
        }

    }
}