//Construir un mensaje que se imprime en la columna índice
public class Trazador{
    String tabs;
    public Trazador(int ntabs, String etiqueta){
        tabs = "";
        for (int i = 0; i < ntabs; i++){ 
            tabs += "\t";
        }
        tabs += "[" + etiqueta + "]";
        
        }
    public void Print(String s){
        System.out.printf("%s%s\n", tabs, s);
    }
}