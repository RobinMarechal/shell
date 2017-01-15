CC=gcc
LIBS=-lreadline -lpthread
EXEC=myshell
all:$(EXEC)
CCFLAGS=-g -Wall

$(EXEC): main.o cmd.o shell_fct.o helpers.o
	gcc $(CCFLAGS) -o  $(EXEC) main.o cmd.o shell_fct.o helpers.o $(LIBS)

helpers.o: helpers.c
	$(CC) $(CCFLAGS) -o helpers.o -c helpers.c

cmd.o: cmd.c
	$(CC)  $(CCFLAGS) -o cmd.o -c cmd.c

shell_fct.o: shell_fct.c
	$(CC)  $(CCFLAGS) -o shell_fct.o -c shell_fct.c

main.o: main.c
	$(CC)  $(CCFLAGS) -o main.o -c main.c

.PHONY: clean

clean:
	rm -vf *.o
