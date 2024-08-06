#!/bin/bash 
g++ datamaker.cpp -o datamaker.out -O3 --std=c++11
g++ -c theheap.cpp --std=c++11 -O3 
g++ -c order.cpp --std=c++11 -O3 
g++ -c thebin.cpp --std=c++11 -O3 
g++ -c trine.cpp --std=c++11 -O3
g++ -c main.cpp --std=c++11 -O3
g++ theheap.o order.o thebin.o trine.o main.o -o K_Trine.out -O3 --std=c++11