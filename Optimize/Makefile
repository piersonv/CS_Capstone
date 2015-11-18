OPTS=-Wall -g -std=c++11
#OPTS=-Wall -g -pg -std=c++11 -O
#OPTS=-Wall -std=c++11 -O3
CC=g++
#LIBS = -lm
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lm

dataCollection: dataCollection.cpp dataCollection.h parser.h dataCollectionDriver.cpp eriolObjs.o NCCDemo.h
	$(CC) $(OPTS) optimize.cpp dataCollection.cpp dataCollectionDriver.cpp eriolObjs.o $(LIBS) -o $@

dataCollection.o: 

Optimize: optimize.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h
	$(CC) $(OPTS) optimize.cpp eriolObjs.o $(LIBS) -o $@
