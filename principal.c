#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_QUANTUM 1
#define TRACE printf("%s:%d\n",__FILE__,__LINE__)
#define PERROR(m){perror(m); exit(EXIT_FAILURE);}
#define PRINTF(men){printf(men); exit(EXIT_FAILURE);}
#define MIN(a,b) ((a < b) ? (a) : (b))


void alarma(int SIGNUM);
pid_t *pid_Hijos;
char **estados;
int senial;
int numHijosCreados = 0;
/*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
char *fin = NULL;
char *fin2 = NULL;
int status;
int q = 0;
int hijosmuertos = 0; 
long int num_hijos = 0;
char n[80];
int pid;
time_t quantum = DEFAULT_QUANTUM;
int comprueba = 0;
long tiempo_ejecucion_terminal = 0;
int tiempo = 0;
int iteracion = 0;
int contadorTiempos = 0;
int fd;
char buf[] = "Prueba...";
int *tiemposEjec_Hijos;
int *burst_time;
int *response_time;
int *turnaround_time;


//Función que gestiona el procesamiento de los argumentos del programa
void procesarArgs(int argc, char *argv[])
{
    if(argc == 1) PRINTF("No hay argumentos que procesar [-n] ó [-q]\n");    
    for(int i = 1; i < argc;)
    {
        tiempo_ejecucion_terminal = strtol(argv[i], &fin, 10);
        if(strcmp(argv[i], "-n") == 0) 
        {
            if (argv[i+1] == NULL) PRINTF("no hay argumentos en [-n] [principal.c]\n");
            /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
            num_hijos = strtol(argv[i+1], &fin, 10); 
            /*si lo que tecleamos por pantalla es una letra o menor que 0, es negativo y por tanto terminamos el programa*/         
            if(num_hijos < 0 || *fin != '\0') PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n"); 
            tiemposEjec_Hijos = (int*) malloc(sizeof(int)* num_hijos);
            burst_time = (int*) malloc(sizeof(int)* num_hijos);
            response_time = (int*) malloc(sizeof(int)* num_hijos);
            turnaround_time = (int*) malloc(sizeof(int)* num_hijos);
            pid_Hijos = (int*) malloc(sizeof(int)* num_hijos);
            estados = (char**) malloc(sizeof(char*)* num_hijos);
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
        else if(*fin == '\0' && tiempo_ejecucion_terminal > 0)
        {
            if(contadorTiempos >= num_hijos)
            {
                free(tiemposEjec_Hijos);
                PRINTF("Demasiados argumentos proporcionados para el número de hijos\n");
            } 
            tiemposEjec_Hijos[contadorTiempos] = tiempo_ejecucion_terminal;
            burst_time[contadorTiempos] = tiempo_ejecucion_terminal;
            contadorTiempos++;
            i++;
        }
        else if(*fin != '\0' || tiempo_ejecucion_terminal < 0){
            PRINTF("tiempo(s) de ejecucion menor que 0 o no es(son) entero(s)\n");
        }else PRINTF("Modificador(es) desconocido [-q] [-n]\n"); 
    }   
    if(contadorTiempos < num_hijos) PRINTF("Faltan argumentos para el número de hijos proporcionado\n");    
    /*comprobamos que se haya introducido el argumento -n*/
    if(comprueba != 1){ free(tiemposEjec_Hijos); PRINTF("Es obligatorio el argumento [-n]\n");}
}



int main(int argc, char *argv[])
{
    
    /*PROCESADOR DE ARGUMENTOS*/
    procesarArgs(argc, argv);
    /*AQUI FINALIZA EL PROCESADOR DE ARGUMENTOS POR TERMINAL*/
    
    /*COMIENZO DE  LA FUNCIONALIDAD DEL PROGRAMA*/
    signal(SIGALRM, alarma);
    for(int i = 1; i <= num_hijos;i++)
    {
        pid_t pid;
        pid = fork();
                    
        if (pid < 0)
        {
            perror("Error al crear un proceso hijo [principal.c]\n");
            numHijosCreados = i;
            break;
        } 
        if(pid == 0)
        {
            sprintf(n, "%d", i);
            estados[i] = "READY";
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
        estados[i] = "STOPPED";
    }
    while(1)
    {
        for(int i = 0; i < numHijosCreados; i++)
        {      
            q = MIN(tiemposEjec_Hijos[i], quantum); 
            if(q == 0)
            {
                continue;
            }
            if(kill(pid_Hijos[i], SIGCONT) == -1) PERROR("Error al enviar la señal de continuar [principal.c]\n");
            estados[i] = "RUNNING";

            /*calculamos response*/
            if(iteracion < numHijosCreados){
                if(i != 0)
                {
                    response_time[i] = tiempo + response_time[i];    
                }
                else
                {
                    response_time[i] = tiempo;
                }
                iteracion++;
            }    
            /*calculamos Turnaround*/
            tiempo = q + tiempo;
            turnaround_time[i] = tiempo;  

            alarm(q);
            tiemposEjec_Hijos[i] -= q;
            /*pausamos el proceso hasta que recibamos una señal*/
            pause();
            if(tiemposEjec_Hijos[i] == 0)
            {
                hijosmuertos++;
                estados[i] = "TERMINATED";
                if(kill(pid_Hijos[i], SIGINT) == -1) PERROR("Error al matar uno de los hijos\n");
                if(hijosmuertos == numHijosCreados) 
                {    
                    fd = open("/usr/lab/alum/0505976/arq2/MYFIFO", O_WRONLY);
                    
                    printf("\nQuantum: %ld\n", quantum);
                    printf("\n");
                    printf("            Burst   Response Turnaround  ESTADO\n");
                    printf("Arg Pid     Time    Time     Time\n");

                    for(int i = 0; i < hijosmuertos; i++)
                    {
                        printf(" %d  %d       %d      %d       %d      %s\n",i+1 , pid_Hijos[i], burst_time[i], response_time[i], turnaround_time[i], estados[i]);
                    }
                    for(int i = 0; i < hijosmuertos; i++)
                    {
                        pid=waitpid(0,&status, WUNTRACED);
                        printf("Hijo con PID %d señalizado por la señal %d: %s\n", pid, status, strsignal(status));
                    }
                    write(fd, buf, sizeof(buf));
                    close(fd);
                    exit(EXIT_SUCCESS);
                }
            }
            else
            {
                if(kill(pid_Hijos[i], SIGSTOP) == -1) PERROR("Error al enviar la señal de stop [principal.c]\n");
                estados[i] = "STOPPED";
            }
        }
    }      
}
void alarma(int Signum){}