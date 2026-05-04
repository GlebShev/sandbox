all: test

test: main.o matrix.o
	g++ -o test main.o matrix.o

main.o: main.cpp matrix.h
	g++ -c main.cpp

matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp

clean: 
	rm -f *.o test