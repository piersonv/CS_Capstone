OPTS=-Wall -g -std=c++11
#OPTS=-Wall -g -pg -std=c++11 -O
#OPTS=-Wall -std=c++11 -O3
CC=g++
#LIBS = -lm
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lm

Optimize: optimize.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h glareReduction.h
	$(CC) $(OPTS) optimize.cpp eriolObjs.o $(LIBS) -o $@

dataCollection: dataCollection.o dataCollectionDriver.o eriolObjs.o NCCDemo.h
	$(CC) $(OPTS) optimize.cpp dataCollection.o eriolObjs.o $(LIBS) -o $@

dataCollection.o: dataCollection.cpp dataCollection.h
	g++ -g -Wall -std=c++11 -c dataCollection.cpp

dataCollectionDriver.o: dataCollectionDriver.cpp dataCollection.h
	g++ -g -Wall -std=c++11 -c dataCollectionDriver.cpp
