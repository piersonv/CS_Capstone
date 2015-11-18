OPTS=-Wall -g -std=c++11
#OPTS=-Wall -g -pg -std=c++11 -O
#OPTS=-Wall -std=c++11 -O3
CC=g++
#LIBS = -lm
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lm

<<<<<<< HEAD:HomographyProgram/showTile/Makefile
dataCollection: dataCollection.cpp dataCollection.h parser.h dataCollectionDriver.cpp eriolObjs.o NCCDemo.h
	$(CC) $(OPTS) showTile.cpp dataCollection.cpp dataCollectionDriver.cpp eriolObjs.o $(LIBS) -o $@

dataCollection.o: 

showTile: showTile.cpp eriolHeader.h eriolObjs.o NCCDemo.h
	$(CC) $(OPTS) showTile.cpp eriolObjs.o $(LIBS) -o $@


=======
Optimize: optimize.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h
	$(CC) $(OPTS) optimize.cpp eriolObjs.o $(LIBS) -o $@
>>>>>>> 72e87670074c0d148ea35cd29acdb9b1b2503f37:Optimize/Makefile
