#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include "functions.h"

//HAY QUE HACER DE 0 -------------------------ESTA MAL----------------------------------------------------------

int main(int argc, char *argv[]){

   
    /*comprobacion de cuantos argumentos escribimos en la terminal*/
    if(argc != 1){
        /*creamos una variable int donde almacenamos el valor de hijos*/
        int num_hijos = atoi(argv[1]);
        /*comprobamos que el numero sea un entero y sea mayor que 0*/
        if(atoi(argv[1]) > 0 && esnumero(argv[1]) != 1 ){
            
            for(int i = 0; i < num_hijos; i++){
                /*creamos procesos*/
                pid_t pid;
                pid = fork(); 

                /*si el proceso es un padre lo finalizamos*/
                if(pid > 0){
                    wait(NULL);
                    //pause();
                    _exit(EXIT_SUCCESS);  
                }
                else{
                    /*aqui comprobamos que el proceso sea un proceso hijo*/
                    if(pid == 0){
                        printf("Soy hijo\n");
                       
                    }
                    else{
                        if(pid < 0){
                           _exit(EXIT_FAILURE);   
                        }
                    }
                }
            }
            
        }
        else{
            /*si el argumento no coincide con las comprobaciones se imprime el error*/
            printf("Argumento no entero o menor/igual a cero\n");
        }   
    }
    else{
        /*si el numero de argumentes es incorrecto imprimimos error*/
        printf("Numero de argumentos incorrectos. Parametro que debe teclear: [Nº Hijos]\n");
    }

    exit(EXIT_SUCCESS);
}

