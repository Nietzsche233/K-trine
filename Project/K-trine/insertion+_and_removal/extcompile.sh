#!/bin/bash 
g++ -c theheap.cpp --std=c++11 -O3 
g++ -c order.cpp --std=c++11 -O3 
g++ -c thebin.cpp --std=c++11 -O3 
g++ -c trine.cpp --std=c++11 -O3
g++ -c extcount.cpp --std=c++11 -O3
g++ theheap.o order.o thebin.o trine.o extcount.o -o ext.out -O3 --std=c++11