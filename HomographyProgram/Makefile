OPTS=-Wall -g
CC=g++
LIBS = -L/home/cg/palantir/levmar/levmar-2.5 -llevmar -lGL -lGLU -lglut -lm

homography: homography.cpp homography.h eriolHeader.h eriolObjs2.o
	$(CC) $(OPTS) -DUSE_GL homography.cpp eriolObjs2.o $(LIBS) -o $@
