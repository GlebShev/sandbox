PROG = my_shell
CC = gcc
CFLAGS = -g -Wall

OBJ = list.o tree.o exec.o main.o

all: $(PROG)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o list.o

tree.o: tree.c tree.h list.h
	$(CC) $(CFLAGS) -c tree.c -o tree.o

exec.o: exec.c exec.h tree.h
	$(CC) $(CFLAGS) -c exec.c -o exec.o

main.o: main.c list.h tree.h exec.h
	$(CC) $(CFLAGS) -c main.c -o main.o

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(PROG)

clean:
	rm -f *.o $(PROG)

run:
	./$(PROG)
