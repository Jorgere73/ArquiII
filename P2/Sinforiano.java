public class Sinforiano extends Hilos{
    private Trazador trazador;
    private static final int tab = 1;
    public Sinforiano()
    {
        trazador = new Trazador(tab, "Sinforiano");
    }
    public void run()
    {
        for(int i = 0;i < 5;i++)
        {
            trazador.Print("hola soy Sinforiano");
            Hilos.Pausa(1000);
        }
    }
    
}

