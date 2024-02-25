CC = gcc

CFLAGS = -Wall 

SRCS = proceso.c principal.c stats_process.c

PROGS = proceso principal stats_process

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) util.c -o $@ $<

clean:
	rm -f $(PROGS)

