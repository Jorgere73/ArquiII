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
#define EXIT {exit(EXIT_SUCCESS);}
#define PRINTF(men){printf(men); exit(EXIT_FAILURE);}
#define MIN(a,b) ((a < b) ? (a) : (b))

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
    int q = 0;
    int hijosmuertos = 0; 
    long int num_hijos = 0;
    char n[10];
    time_t quantum = DEFAULT_QUANTUM;
    int comprueba = 0;
    long tiempos_ejecucion_terminal = 0;
    int *tiemposEjec_Hijos;
    int *burst_time;
    int *response_time;
    int *turnaround_time;
    int *rt;
    int tiempo = 0;
    int contadorTiempos = 0;

    /*PROCESADOR DE ARGUMENTOS*/
    if(argc == 1) PRINTF("No hay argumentos que procesar [-n] ó [-q]\n");    
    for(int i = 1; i < argc;)
    {
        tiempos_ejecucion_terminal = strtol(argv[i], &fin, 10);
        if(strcmp(argv[i], "-n") == 0) 
        {
            if (argv[i+1] == NULL) PRINTF("no hay argumentos en [-n] [principal.c]\n");
            /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
            num_hijos=strtol(argv[i+1], &fin, 10); 
            /*si lo que tecleamos por pantalla es una letra o menor que 0, es negativo y por tanto terminamos el programa*/         
            if(num_hijos < 0 || *fin != '\0') PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n"); 
            tiemposEjec_Hijos = (int*) malloc(sizeof(int)* num_hijos);
            burst_time =(int*) malloc(sizeof(int)* num_hijos);
            response_time = (int*) malloc(sizeof(int)* num_hijos);
            turnaround_time = (int*) malloc(sizeof(int)* num_hijos);
            rt = (int*) malloc(sizeof(int)* num_hijos);
            comprueba = 1;
            i+=2;
        }
        else if(strcmp(argv[i], "-q") == 0)
        {
            if(argv[i+1] == NULL) PRINTF("No hay argumentos que procesar en [-q]\n");
            quantum = strtol(argv[i+1], &fin2, 10);
            if(*fin2 != '\0' || quantum < 0) PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n");
            i+=2;
        }
        else if(*fin == '\0' && tiempos_ejecucion_terminal > 0)
        {
            if(contadorTiempos >= num_hijos)
            {
                free(tiemposEjec_Hijos);
                PRINTF("Demasiados argumentos proporcionados para el número de hijos\n");
            } 
            tiemposEjec_Hijos[contadorTiempos] = tiempos_ejecucion_terminal;
            burst_time[contadorTiempos] = tiempos_ejecucion_terminal;
            contadorTiempos++;
            i++;
        }
        else if(*fin != '\0' || tiempos_ejecucion_terminal < 0){
            PRINTF("tiempo(s) de ejecucion menor que 0 o no es(son) entero(s)\n");
        }else PRINTF("Modificador(es) desconocido [-q] [-n]\n"); 
    }   
    /*comprobamos que se haya introducido el argumento -n*/
    if(comprueba != 1){ free(tiemposEjec_Hijos); PRINTF("Es obligatorio el argumento [-n]\n");}
    /*AQUI FINALIZA EL PROCESADOR DE ARGUMENTOS POR TERMINAL*/
    
    /*COMIENZO DE  LA FUNCIONALIDAD DEL PROGRAMA*/
    comprueba = 0;
    signal(SIGALRM, alarma);
    pid_t pid_Hijos[num_hijos];
    for(int i = 1; i <= num_hijos;i++)
    {
        pid_t pid;
        pid = fork();
                    
        if (pid < 0){
            perror("Error al crear un proceso hijo [principal.c]\n");
            numHijosCreados = i;
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
    if(numHijosCreados < num_hijos) PERROR("Error en creacion, numero de hijos menor que parametro escrito en terminal [principal.c]\n");
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
            q = MIN(tiemposEjec_Hijos[i], quantum); 

            if(q == 0)
            {
                continue;
            }
            rt[i]=q;
            if(comprueba != 1){
                response_time[i] = tiempo;
                comprueba = 1;
            }
            else 
            {
                
                response_time[i] = tiempo;
            }
            if(tiemposEjec_Hijos[i] > quantum)
            {
                tiempo=quantum+tiempo;
            }
            else if(tiemposEjec_Hijos[i]!=0)
            {
                tiempo= tiempo +tiemposEjec_Hijos[i];
                turnaround_time[i]=tiempo;
            }
            alarm(q);
            tiemposEjec_Hijos[i] -= q;
            /*pausamos el proceso hasta que recibamos una señal*/
            pause();
            if(tiemposEjec_Hijos[i] == 0)
            {
                hijosmuertos++;
                if(kill(pid_Hijos[i], SIGKILL) == -1) PERROR("Error al matar uno de los hijos\n");
                if(hijosmuertos == numHijosCreados) 
                {    
                    printf("\nQuantum: %ld\n", quantum);
                    printf("\n");
                    printf("            Burst   Response Turnaround  ESTADO\n");
                    printf("Arg Pid     Time    Time     Time\n");
                    for(int i = 0; i < hijosmuertos; i++)
                    {
                        printf(" %d  %d       %d      %d       %d      TERMINATED\n",i+1 , pid_Hijos[i], burst_time[i], response_time[i], turnaround_time[i]);
                    }
                    EXIT;
                }
            }
            else
            {
                turnaround_time[i]=tiempo + tiemposEjec_Hijos[i];
            }
           
           
            if(kill(pid_Hijos[i], SIGSTOP) == -1) PERROR("Error al enviar la señal de stop [principal.c]\n");
        }
    }       
}