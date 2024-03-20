public class Maruxa extends Hilos{
    private Trazador trazador;
    private static final int tab = 2;
    public Maruxa()
    {
        trazador = new Trazador(tab, "Maruxa");
    }
    public void run()
    {
        for(int i = 0;i < 5;i++)
        {
            trazador.Print("hola soy Maruxa");
            Hilos.Pausa(1000);
        }
    }
    
}