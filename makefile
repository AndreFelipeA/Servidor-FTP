CC      = gcc
CFLAGS  = -Wall
PROGRAM = main
LIB     = lib


all:
	$(CC) $(CFLAGS) -c $(LIB)/*.h
	$(CC) $(CFLAGS) -c $(LIB)/*.c
	$(CC) $(CFLAGS) -c *.c
	$(CC) *.o -o $(PROGRAM)

run:
	valgrind ./$(PROGRAM)

clean:
	rm *.o $(LIB)/*.gch

