#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define DEFAULT_QUANTUM 1
/*lo declaramos como variable global para poder usarlo en la funcion que usamos signal*/
time_t quantum = DEFAULT_QUANTUM;

void alarma(int SIGNUM);
int main(int argc, char *argv[]){


        /*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
        char *fin = NULL;
        char *fin2 = NULL;
        int numHijosCreados = 0;
        /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
        long int num_ter=strtol(argv[1], &fin, 10);

            if(argv[2] != NULL)
            {
                quantum = strtol(argv[2], &fin2, 10);
                if(*fin2 != '\0')
                {
                    perror("Argumento(s) no es entero o menor que 0 [principal.c]");
                    exit(EXIT_FAILURE); 
                }
            }
            if (argv[1] == NULL)
            {
                /*Si recibimos una cadena vacia, imprimimos error*/
                printf("no hay argumentos [principal.c]\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                /*si el numero tecleado por pantalla es mayor que 0, es positivo y por tanto seguimos con el programa ademas hacemos la comprobacion de si contiene un caracter*/
                if(num_ter > 0 && *fin == '\0')
                {
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
                            /*no entiendo como funciona el almacenamiento y envío*/
                            //char n[10];
                            char *n = malloc((i+1)*sizeof(char));
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
                            if(kill(pid_Hijos[i], SIGSTOP) == -1)
                            {
                                perror("Error al enviar la señal de stop [principal.c]\n");
                            }
                            else
                            {
                                printf("Proceso hijo %d parado con éxito\n", pid_Hijos[i]);
                            }
                        }
                        /*iniciamos el cronometro con alarm y con quantum indicamos cuantos segundos queremos que pasen*/
                        alarm(quantum);
                        
                        while(1)
                        {

                            for(int i = 0; i < numHijosCreados; i++)
                            {                                    
                                    if(kill(pid_Hijos[i], SIGCONT) == -1)
                                    {
                                        perror("Error al enviar la señal de continuar [principal.c]\n");
                                    }
                                    else{
                                        /*usamos signal para que cuando recibamos una señal alarm, haga la funcion alarma donde reiniciamos el contador alarm*/
                                        signal(SIGALRM, alarma);
                                        /*pausamos el proceso hasta que recibams la señal ALARM*/
                                        pause();
                                        
                                        if(kill(pid_Hijos[i], SIGSTOP) == -1)
                                        {
                                            perror("Error al enviar la señal de stop [principal.c]\n");
                                        } 
                                        
                                    }
                            }
                        }       
                        //Salimos con señal correcta
                        exit(EXIT_SUCCESS);
                    }
                    
                }
                else
                {
                    perror("Argumento(s) no es entero o menor que 0 [principal.c]");
                    exit(EXIT_FAILURE);
                }  
            }
}

void alarma(int Signum)
{
    /*reiniciamos alarm*/
    alarm(quantum);
}