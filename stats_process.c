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
    FILE *fd;
    size_t num_bytes;
    char buf[MAX_SIZE];

    if(mkfifo("./MYFIFO", 0666) == -1)
    {
        if(errno == EEXIST)
        {
            remove("./MYFIFO");
        }
        else
        {
            PERROR("Error al crear el pipe");
        }
    }
    fd = fopen("./MYFIFO", "r");
    if(fd == NULL) PERROR("Error al abrir la tuberia\n");

    num_bytes = fread(buf, sizeof(char), MAX_SIZE, fd);
    if(num_bytes == -1) PERROR("Error al leer bytes de la tuberia\n");

    printf("Numeros de bytes recibidos: %d\n", num_bytes);
    printf("Mensaje recibido: %s\n",buf);
    fclose(fd);
    remove("./MYFIFO");
    exit(EXIT_SUCCESS);
}