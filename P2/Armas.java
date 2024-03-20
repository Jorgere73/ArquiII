public enum Arma {
    FIN_SIMULACION, FIN_LOTE,
    MOSQUETE, LANZA, ALABARDA, TRABUCO;
    private static Arma[] valores = values();
    public static Arma Aleatoria()
    {
        int i = (int)(Math.random()*(valores.length-2)+2 );
        return valores[i];
    }
}