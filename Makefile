CC=g++
CFLAGS = -lm

main.o: main.cpp
	$(CC) main.cpp -o main.o

clean:
	rm *.o 
