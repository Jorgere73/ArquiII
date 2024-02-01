#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>

//HAY QUE HACER DE 0 -------------------------ESTA MAL----------------------------------------------------------

int main(int argc, char *argv[]){


        /*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
        char *fin = NULL;

        /*se crea un string para almacenar la concatenacion de "+" en funcion del numero recibido por parametro en el terminal*/ 
         
        /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
        long int num_ter=strtol(argv[1], &fin, 10);
        int numHijosCreados = 0;

        
            if (argv[1] == NULL){
                /*Si recibimos una cadena vacia, imprimimos error*/
                printf("no hay argumentos [principal.c]\n");
            }
            else{
                /*si el numero tecleado por pantalla es mayor que 0, es positivo y por tanto seguimos con el programa ademas hacemos la comprobacion de si contiene un caracter*/
                if(num_ter > 0 && *fin == '\0' ){
                    pid_t pid_Hijos[num_ter];
                    for(int i = 1; i<=num_ter;i++){
                        /*creamos los procesos*/    
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

                        if(pid == 0){
                            /*no entiendo como funciona el almacenamiento y envío*/
                            
                            //char n[10];
                            char *n = malloc((i+1)*sizeof(char));
                            sprintf(n, "%d", i);
                            execl("./proceso","proceso",n, (char *)NULL); 
                        }
                        else{
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
                    else{
                    //sleep(5);
                        for (int i = 0; i < numHijosCreados; i++) {
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
                        //Salimos con señal correcta
                        exit(EXIT_SUCCESS);
                    }   
                }
                else{
                    perror("Argumento no es entero o menor que 0 [principal.c]");
                    exit(EXIT_FAILURE);
                }               
            }
}
