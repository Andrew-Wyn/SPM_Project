.PHONY: clean

FLAGS = -g -pthread -std=c++20 -O3
COMP = g++

all: clean main

main: main.o utils.o utimer.o seq_jacobi.o ff_jacobi.o th_jacobi.o th2_jacobi.o
	$(COMP) -o main main.o utils.o utimer.o seq_jacobi.o th_jacobi.o th2_jacobi.o ff_jacobi.o $(FLAGS)

main.o: main.cpp utils.hpp utimer.hpp seq_jacobi.hpp th_jacobi.hpp th2_jacobi.hpp ff_jacobi.hpp
	$(COMP) -c main.cpp $(FLAGS)

utils.o: utils.cpp utils.hpp 
	$(COMP) -c utils.cpp $(FLAGS)

utimer.o: utimer.cpp utimer.hpp 
	$(COMP) -c utimer.cpp $(FLAGS)

seq_jacobi.o: seq_jacobi.cpp seq_jacobi.hpp utils.hpp 
	$(COMP) -c seq_jacobi.cpp $(FLAGS)

ff_jacobi.o: ff_jacobi.cpp ff_jacobi.hpp utils.hpp 
	$(COMP) -c ff_jacobi.cpp $(FLAGS)

th_jacobi.o: th_jacobi.cpp th_jacobi.hpp utils.hpp 
	$(COMP) -c th_jacobi.cpp $(FLAGS)

th2_jacobi.o: th2_jacobi.cpp th2_jacobi.hpp utils.hpp 
	$(COMP) -c th2_jacobi.cpp $(FLAGS)

clean:
	-rm main
	-rm *.o