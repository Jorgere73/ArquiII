#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_QUANTUM 1
#define TRACE printf("%s:%d\n",__FILE__,__LINE__)
#define PERROR(m){perror(m); exit(EXIT_FAILURE);}
#define PRINTF(men){printf(men); exit(EXIT_FAILURE);}
#define MIN(a,b) ((a < b) ? (a) : (b))
#define MAX_SIZE 5000

void procesarArgs(int argc, char *argv[]);
void creacionProcesos();
void procesamientoPrincipal();
void alarma(int Signum);

#endif