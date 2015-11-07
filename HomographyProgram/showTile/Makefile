OPTS=-Wall -g -std=c++11
#OPTS=-Wall -g -pg -std=c++11 -O
#OPTS=-Wall -std=c++11 -O3
CC=g++
#LIBS = -lm
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lm

showTile: showTile.cpp eriolHeader.h eriolObjs.o NCCDemo.h
	$(CC) $(OPTS) showTile.cpp eriolObjs.o $(LIBS) -o $@
