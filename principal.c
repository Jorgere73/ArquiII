#include "util.h"

int main(int argc, char *argv[])
{
    
    /*PROCESADOR DE ARGUMENTOS*/
    procesarArgs(argc, argv);
    /*AQUI FINALIZA EL PROCESADOR DE ARGUMENTOS POR TERMINAL*/
    
    /*COMIENZO DE  LA FUNCIONALIDAD DEL PROGRAMA*/
    creacionProcesos();
    while(1)
    {
        procesamientoPrincipal();
    }
}