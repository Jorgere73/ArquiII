#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define DEFAULT_QUANTUM 1
#define TRACE printf("%s:%d\n",__FILE__,__LINE__)
#define PERROR(m){perror(m); exit(EXIT_FAILURE);}
#define PRINTF(men){printf(men); exit(EXIT_FAILURE);}

/*lo declaramos como variable global para poder usarlo en la funcion que usamos signal*/
time_t quantum = DEFAULT_QUANTUM;
void alarma(int SIGNUM);

void alarma(int Signum)
{
    //incrementaremos contador
}

int main(int argc, char *argv[]){

    /*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
    char *fin = NULL;
    char *fin2 = NULL;
    int numHijosCreados = 0;
    long int num_hijos = 0;
    char n[10];

    if(argc == 1) PRINTF("No hay argumentos que procesar [-n] ó [-q]\n");    
    for(int i = 1; i < argc;){
        if(strcmp(argv[i], "-n") == 0) 
        {
            if (argv[i+1] == NULL) PRINTF("no hay argumentos [principal.c]\n");
            /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
            num_hijos=strtol(argv[i+1], &fin, 10); 
            /*si lo que tecleamos por pantalla es una letra o menor que 0, es negativo y por tanto terminamos el programa*/         
            if(num_hijos < 0 || *fin != '\0') PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n"); 
            i+=2;
        }
        else if(strcmp(argv[i], "-q") == 0)
        {
            if(argv[i+1] == NULL) PRINTF("No hay argumentos que procesar en [-q]\n");
            quantum = strtol(argv[i+1], &fin2, 10);
            if(*fin2 != '\0' || quantum < 0) PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n")
            i+=2;
        }
        else PRINTF("Modificador(es) desconocido [-q] [-n]\n"); 
    }
    
    
    signal(SIGALRM, alarma);
    pid_t pid_Hijos[num_hijos];
    for(int i = 1; i <= num_hijos;i++)
    {
        pid_t pid;
        pid = fork();
                    
        if (pid < 0) 
        {
            perror("Error al crear un proceso hijo [principal.c]\n");
            //Guardamos cuántos procesos se han creado, para pararlos con kill()
            numHijosCreados = i;
            //Salimos del for()
            break;
        }
        if(pid == 0)
        {
            sprintf(n, "%d", i);
            execl("./proceso","proceso",n, (char *)NULL); 
        }
        else
        {
            //Dentro del proceso padre, guardamos los pids de los procesos hijos
            pid_Hijos[i-1] = pid;
            //Si no hay errores, numHijosCreados será igual a numHijos
            numHijosCreados = i;
        }
    }

    //Si el número de hijos creados fue menor al pasado por parámetro, hubo un error en la creación
    if(numHijosCreados < num_hijos) PERROR("Error en creacion, numero de hijos menor que parametro escrito en terminal [principal.c]\n")
    for (int i = 0; i < numHijosCreados; i++)
    {
        //Envíamos señal SIGSTOP a cada uno de los procesos hijos y comprobamos si da error
        /*kill nos sirve para enviar una señal, en esta caso con SIGSTOP paramos los procesos, pero no los matamos*/
        if(kill(pid_Hijos[i], SIGSTOP) == -1) PERROR("Error al enviar la señal de stop [principal.c]\n");
    }
    while(1)
    {
        for(int i = 0; i < numHijosCreados; i++)
        {                                    
            if(kill(pid_Hijos[i], SIGCONT) == -1) PERROR("Error al enviar la señal de continuar [principal.c]\n");
            alarm(quantum);
            /*pausamos el proceso hasta que recibamos una señal*/
            pause();      
            if(kill(pid_Hijos[i], SIGSTOP) == -1) PERROR("Error al enviar la señal de stop [principal.c]\n");
        }
    }       
    //Salimos con señal correcta
    exit(EXIT_SUCCESS);
}