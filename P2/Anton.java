import java.util.Random;
public class Anton extends Hilos{

    public boolean estoy_vivo = true;
    private static int tiempo_remando_anton = 0;
    private Trazador trazador;
    private static final int tab = 3;
    public Anton()
    {
        trazador = new Trazador(tab, "Anton");
    }
    public void run()
    {
       
        while(estoy_vivo)
        { 
            
            trazador.Print("INICIO");
            trazador.Print("En el embarcadero");
            Hilos.Pausa(Constantes.TESPERA_EMBARCADERO);
            trazador.Print("Yendo con:(AQUI HAY QUE METER PASAJEROS)");
            remando();
            trazador.Print("En la playa");
            Hilos.Pausa(Constantes.TESPERA_PLAYA);
            trazador.Print("Volviendo con:(LOS PASAJEROS QUE SEAN)");
            remando();
            
        }
        

    }
    public void remando()
    {
        Random random = new Random();
        trazador.Print("Remando");
        tiempo_remando_anton = random.nextInt(Constantes.TMAX_TRAVESIA - Constantes.TMIN_TRAVESIA + 1) + Constantes.TMIN_TRAVESIA ;
        Hilos.Pausa(tiempo_remando_anton);
    }

}