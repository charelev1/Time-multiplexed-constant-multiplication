CC=g++
CFLAGS = -lm

main.o: main.cpp
	$(CC) main.cpp -o main.o

init: table_init.cpp
	$(CC) table_init.cpp -o table_init.o

clean:
	rm *.o 
