OPTS=-Wall -g -std=c++11
#OPTS=-Wall -g -pg -std=c++11 -O
#OPTS=-Wall -std=c++11 -O3
CC=g++
#LIBS = -lm
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lm

Optimize: optimize.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h optimize.h glareReduction.h dataCollection.o
	$(CC) $(OPTS) optimize.cpp dataCollection.o eriolObjs.o $(LIBS) -o $@
testall: testall.cpp eriolHeader.h eriolObjs.o 
	$(CC) $(OPTS) testall.cpp eriolObjs.o $(LIBS) -o $@

dataCollection.o: dataCollection.cpp dataCollection.h
	g++ -g -Wall -std=c++11 -c dataCollection.cpp

Optimize-Test: optimize-test.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h glareReduction.h
	$(CC) $(OPTS) optimize-test.cpp eriolObjs.o $(LIBS) -o $@

Optimize-test_CUDA: optimize-test_CUDA.cpp eriolHeader.h eriolObjs.o NCCDemo.h homography.h
	/usr/local/cuda/bin/nvcc $(LIBS) -std=c++11 eriolObjs.o -o optimize-test_CUDA optimize-test_CUDA.cu
