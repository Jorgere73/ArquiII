#include "util.h"

pid_t *pid_Hijos;
char **estados;
int numHijosCreados = 0;
long int num_hijos = 0;
time_t quantum = DEFAULT_QUANTUM;
int *tiemposEjec_Hijos;
int *burst_time;
int *response_time;
int *turnaround_time;
int hijosmuertos = 0; 

//Función que gestiona el procesamiento de los argumentos del programa
void procesarArgs(int argc, char *argv[])
{
    /*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
    char *fin = NULL;
    int comprueba = 0;
    int contadorTiempos = 0;
    long tiempo_ejecucion_terminal = 0;
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
            quantum = strtol(argv[i+1], &fin, 10);
            if(*fin != '\0' || quantum < 0) PRINTF("Argumento(s) no es entero o menor que 0 [principal.c]\n");
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

void creacionProcesos()
{
    char n[80];
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
}

void procesamientoPrincipal()
{
    int pid;
    int status;
    int q = 0;
    int tiempo = 0;
    int iteracion = 0;
    //Descriptor del pipe al que escribiremos (MYFIFO)
    FILE *fd;
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
                response_time[i] += tiempo;    
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
            size_t bytesEscritos = 0;
            hijosmuertos++;
            estados[i] = "TERMINATED";
            if(kill(pid_Hijos[i], SIGINT) == -1) PERROR("Error al matar uno de los hijos\n");
            printf("%d, %d\n", hijosmuertos, numHijosCreados);
            sleep(2);
            if(hijosmuertos == numHijosCreados) 
            {    
                fd = fopen("./MYFIFO", "w");
                //String que contendrá el texto a escribir al pipe
                char estadisticas[MAX_SIZE];
                //Guarda en estadisticas la tabla inicial sin valores aún
                sprintf(estadisticas, "Quantum: %ld\n\n            Burst   Response Turnaround  ESTADO\nArg Pid     Time    Time     Time\n", quantum);
                for(int i = 0; i < hijosmuertos; i++)
                {
                    char temp[MAX_SIZE];
                    //Escribimos en una variable temporal los valores de los parámetros para cada hijo
                    sprintf(temp, " %d  %d       %d      %d       %d      %s\n",i+1 , pid_Hijos[i], burst_time[i], response_time[i], turnaround_time[i], estados[i]);
                    //Concatenamos estos strings con la tabla de estadísticas inicial
                    strcat(estadisticas, temp);
                }
                //Imprimimos la tabla de estadísticas completa por pantalla
                printf("%s", estadisticas);
                for(int i = 0; i < hijosmuertos; i++)
                {
                    pid = waitpid(0,&status, WUNTRACED);
                    printf("Hijo con PID %d señalizado por la señal %d: %s\n", pid, status, strsignal(status));
                }
                //Escribimos el valor contenido en el buffer al pipe
                bytesEscritos = fwrite(estadisticas, sizeof(char), MAX_SIZE, fd);
                if(bytesEscritos == 0) PERROR("Error al escribir sobre el pipe");
                //Cerramos el pipe
                fclose(fd);
                //Liberamos los mallocs
                free(tiemposEjec_Hijos);
                free(burst_time);
                free(response_time);
                free(turnaround_time);
                free(pid_Hijos);
                free(estados);
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            if(kill(pid_Hijos[i], SIGSTOP) == -1) PERROR("Error al enviar la señal de stop [principal.c]\n");
            estados[i] = "STOPPED";
        }
    }    
    printf("sale\n");
}

void alarma(int Signum){}