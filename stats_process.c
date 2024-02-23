#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define PERROR(m){perror(m); exit(EXIT_FAILURE);}
#define PRINTF(men){printf(men); exit(EXIT_FAILURE);}
#define MAX_SIZE 5000

int main()
{
    //Descriptor de archivo para el pipe
    FILE *fd;
    //Numero de bytes recibidos/leídos en el pipe
    size_t num_bytes;
    //Buffer donde guardaremos el contenido del interior del pipe
    char buf[MAX_SIZE];

    //Creamos fifo con permisos de lectura y de escritura y comprobamos que no hay error en la creación
    if(mkfifo("./MYFIFO", 0666) == -1)
    {
        //Comprobamos que no se produce un error porque el archivo MYFIFO ya existía
        if(errno == EEXIST)
        {
            //En caso de que sí existiera, lo eliminamos
            remove("./MYFIFO");
        }
        else
        {
            //Para cualquier otro error...
            PERROR("Error al crear el pipe");
        }
    }
    //Abrimos el pipe en modo de lectura
    fd = fopen("./MYFIFO", "r");
    //Comprobamos que no hay error en la apertura del pipe
    if(fd == NULL) PERROR("Error al abrir la tuberia\n");

    //Procedemos a leer el contenido del pipe (espera si está vacío)
    num_bytes = fread(buf, sizeof(char), MAX_SIZE, fd);
    //Comprobamos errores durante la lectura
    if(num_bytes == 0) PERROR("Error al leer bytes de la tuberia\n");

    //Imprimimos el tamaño de bytes y la información recibida
    printf("Numeros de bytes recibidos: %ld\n", num_bytes);
    printf("Mensaje recibido: %s\n",buf);
    //Cerramos el pipe
    fclose(fd);
    //Borramos el archivo del pipe
    remove("./MYFIFO");
    exit(EXIT_SUCCESS);
}