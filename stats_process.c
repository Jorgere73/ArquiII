#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define PERROR(m){perror(m); exit(EXIT_FAILURE);}
#define MAX_SIZE 5000

int main()
{
    FILE *fd;
    size_t num_bytes;
    char buf[MAX_SIZE];

    if(mkfifo("./MYFIFO", 0666) != 0) PERROR("Error al crear fifo\n");
    fd = fopen("./MYFIFO", "r");
    if(fd == NULL) PERROR("Error al abrir la tuberia\n");

    printf("Aa\n");
    num_bytes = fread(buf, sizeof(char), MAX_SIZE, fd);
    if(num_bytes == -1) PERROR("Error al leer bytes de la tuberia\n");

    printf("Numeros de bytes recibidos: %d\n", num_bytes);
    printf("Mensaje recibido: %s\n",buf);
    fclose(fd);
    remove("./MYFIFO");
    exit(EXIT_SUCCESS);
}