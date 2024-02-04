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
    long int num_ter = 0;
    char n[10];

    if(argc == 1){
        printf("No hay argumentos que procesar [-n] ó [-q]");
        exit(EXIT_FAILURE);
    } 
    for(int i = 1; i < argc;){
        if(strcmp(argv[i], "-n") == 0)
        {
            if (argv[i+1] == NULL)// verificar si el argumento existe argc
            {
                /*Si recibimos una cadena vacia, imprimimos error*/
                printf("no hay argumentos [principal.c]\n");
                TRACE;
                exit(EXIT_FAILURE);
            } 
            else
            {
                /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
                num_ter=strtol(argv[i+1], &fin, 10);
                i+=2;
            }    
        }
        else if(strcmp(argv[i], "-q") == 0)
        {
                if(argv[i+1] == NULL) //mirar luego estructura
                {
                    printf("no hay argumentos [principal.c]\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    quantum = strtol(argv[i+1], &fin2, 10);
                    if(*fin2 != '\0' || quantum < 0) PERROR("Argumento(s) no es entero o menor que 0 [principal.c]\n")
                    i+=2;
                }
        }
        else
        {
            perror("Modificador desconocido [-q] [-n]\n");
            exit(EXIT_FAILURE);         
        }    
    }
    
    /*si el numero tecleado por pantalla es mayor que 0, es positivo y por tanto seguimos con el programa ademas hacemos la comprobacion de si contiene un caracter*/
    if(num_ter > 0 && *fin == '\0')
    {
        signal(SIGALRM, alarma);
        pid_t pid_Hijos[num_ter];
        for(int i = 1; i <= num_ter;i++)
        {
            pid_t pid;
            pid = fork();
                        
            if (pid < 0) 
            {
                perror("Error al crear un proceso hijo [principal.c]");
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
        if(numHijosCreados < num_ter)
        {
            //Salimos con señal de error
            exit(EXIT_FAILURE);
        }
        else
        {
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
    }
    else
    {
        perror("Argumento(s) no es entero o menor que 0 [-n (obligatorio)] [principal.c]");
        exit(EXIT_FAILURE);
    }               
}