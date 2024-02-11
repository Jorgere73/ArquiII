#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define PERROR(m){perror(m); exit(EXIT_FAILURE);}

int main()
{
    int fd;
    int num_bytes;
    char buf[1024];

    if(mkfifo("/home/student/arq22.0/MYFIFO", 0666) != 0) PERROR("Error al crear fifo\n");
    fd = open("/home/student/arq22.0/MYFIFO", O_RDONLY);
    if(fd == -1) PERROR("Error al abrir la tuberia\n");

    num_bytes = read(fd, buf, sizeof(buf));
    if(num_bytes == -1) PERROR("Error al leer bytes de la tuberia\n");
    
    printf("Numeros de bytes recibidos: %d\n", num_bytes);
    printf("Mensaje recibido: %s\n",buf);
    close(fd);

    exit(EXIT_SUCCESS);
}