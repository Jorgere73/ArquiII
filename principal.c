#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    //Comprobamos que se haya pasado un parámetro
    if(argc > 1)
    {
        //Puntero que apuntará a un valor '\0' si el argumento del programa es un número
        char *fin;
        //Convertimos argv[1] a long, en base decimal, y si todo correcto: *fin = '\0'
        //Número de procesos hijo que vamos a tener que crear
        long numHijos = strtol(argv[1], &fin, 10);
        //Número de hijos creados hasta que uno falla, para saber cuántos procesos parar en caso de error
        int numHijosCreados = 0;
        //Comprobamos que el parámetro sea un número, y sea positivo
        if(numHijos > 0 && (*fin == '\0'))
        {
            //Array que guarda los pids de los procesos hijos para envío de señales
            pid_t pidHijos[numHijos];
            for (int i = 1; i < numHijos+1; i++) 
            {

                // Creamos el número de procesos que indica el argumento del programa
                pid_t pid = fork();
                
                // Comprobamos que no haya errores al crear los procesos
                if (pid < 0) 
                {
                    perror("Error al crear un proceso hijo");
                    //Guardamos cuántos procesos se han creado, para pararlos con kill()
                    numHijosCreados = i;
                    //Salimos del for()
                    break;
                }

                // Disociamos el proceso principal de hijos
                else if (pid == 0) {
                    //Guarda la conversión de i a str
                    char n[5];  
                    //Convertimos i en str, guardándolo en n
                    sprintf(n, "%d", i);
                    //Creamos el array de parámetros para proceso.c
                    char *args[] = {"./proceso", n, NULL};
                    //Ejecutamos el programa para el proceso hijo actual
                    execvp("./proceso", args);
                    //Si la ejecución del programa falla, devolvemos error
                    perror("Error al ejecutar el programa");
                    //Salimos del programa con señal de error
                    exit(EXIT_FAILURE);
                }
                else
                {
                    //Dentro del proceso padre, guardamos los pids de los procesos hijos
                    pidHijos[i-1] = pid;
                    //Si no hay errores, numHijosCreados será igual a numHijos
                    numHijosCreados = i;
                }
            }
            //sleep(2);
            //Recorremos el for() para parar únicamente los procesos iniciados correctamente (numHijosCreados)
            for (int i = 0; i < numHijosCreados; i++) {
                //Envíamos señal SIGSTOP a cada uno de los procesos hijos y comprobamos si da error
                if(kill(pidHijos[i], SIGSTOP) == -1)
                {
                    perror("Error al enviar la señal de stop\n");
                }
                else
                {
                    printf("Proceso hijo %d parado con éxito\n", pidHijos[i]);
                }
            }

            exit(EXIT_SUCCESS);
        }
    }
    else
    {
        printf("No se han introducido suficientes argumentos\n");
        exit(EXIT_FAILURE);
    }
}
