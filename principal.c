#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

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
        //Comprobamos que el parámetro sea un número, y sea positivo
        if((atoi(argv[1]) > 0) && (*fin == '\0'))
        {
            for(int i = 0; i < numHijos; i++)
            {
                //Creamos el número de procesos que indica el argumento del programa
                pid_t pid = fork();

                //Comprobamos que no haya errores al crear los procesos
                if(pid < 0)
                {
                    perror("Error al crear un proceso hijo");
                    exit(-1);
                }

                //Disociamos el proceso principal de hijos
                if(pid == 0)
                {
                
                }
                }
            
        }
    }
    else
    {
        printf("No se han introducido suficientes argumentos\n");
    }
}