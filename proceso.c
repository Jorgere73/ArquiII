#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){

    if(argc != 1){
        /*se crea una variable int para saber el numero de iteraciones que tenemos*/
        int num_iteracion = 0;
        /*creo un puntero para usarlo en strtol que me indicara si cuando lo hemos pasado por dicha funcion ha llegado al final (\0) o por si al contrario, apunta a un caracter*/
        char *fin;
        /*se crea un string para almacenar la concatenacion de "+" en funcion del numero recibido por parametro en el terminal*/ 
        char *cadena_sum; 


        /*esta funcion me pasa el numero que paso por parametro a long int y el puntero fin apunta a la ultima direccion de memoria*/
        long int num_ter=strtol(argv[1], &fin, 10);
        /*aqui almacenaremos los + y le hacemos una reserva dinamica de memoria con un calloc*/
        cadena_sum = malloc(num_ter+1*sizeof(char));
        /*una vez hecha la reserva dinamica añadimos con el memset la cantidad de + que indicamos por parametro en el terminal, almacenandolos en la variable donde hicimos la reserva dinamica*/
        memset(cadena_sum, '+', num_ter);

        if (argv[1] == NULL){
            /*Si recibimos una cadena vacia, imprimimos error*/
            printf("no suficientes argumentos\n");
        }
        else{
            /*si el numero tecleado por pantalla es mayor que 0, es positivo y por tanto seguimos con el programa ademas hacemos la comprobacion de si contiene un caracter*/
            if(atoi(argv[1]) > 0 && *fin == '\0' ){
                        
                while (1){
                    
                        /*hacemos la impresion de lo pedido en el enunciado*/
                        printf("[%d] Proceso %ld %s iter    %d\n", getpid(), num_ter, cadena_sum, num_iteracion);
                        /*aumentamos en uno el contador*/ 
                        num_iteracion += 1;     
                        sleep(1);
                }
            }  
            else{ 
                /*en cualquier otro caso en el que el numero no sea positivo o tenga caracteres imprimimos un error*/
                printf("Argumento no entero o menor que 0\n");
                /*al haber error salimos del programa con un exit*/
                exit(0); 
            }
                    
        }
               
    }
    else{
        /*si el numero de parametros introducido es distinto de 1 hay un error*/
        printf("no suficientes argumentos\n");
    }
    
    /*finaliza programa*/
    exit(EXIT_SUCCESS);
}