CC = g++
CFLAGS = -Wall

all: clean sem.o consumer.out producer.out readerA.out readerB.out
	g++ main.cpp -c
	g++ main.o sem.o -o runner.o

sem.o:
	g++ sem.cpp -c

consumer.out:
	g++ consumer.cpp -c
	g++ consumer.o sem.o -o consumer.out
producer.out:
	g++ producer.cpp -c
	g++ producer.o sem.o -o producer.out
	
readerA.out:
	g++ readerA.cpp -c
	g++ readerA.o sem.o -o readerA.out

readerB.out:
	g++ readerB.cpp -c
	g++ readerB.o sem.o -o readerB.out

clean:
	rm -f *.o *.out 

