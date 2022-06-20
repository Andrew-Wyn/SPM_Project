.PHONY: clean

FLAGS = -pthread -std=c++20 -O3
COMP = g++

all: clean main

main:
	$(COMP) -o main main.cpp $(FLAGS)

clean:
	-rm main