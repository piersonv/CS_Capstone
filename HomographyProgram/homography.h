#ifndef _HOMOGRAPHY_
#define _HOMOGRAPHY_

#include <iostream>
#include <time.h>
#include <stdlib.h>
float*  homography(float* a, float* b){
  float * temp = new float[2];
  temp[0] = a[0]*b[0] + a[1]*b[1] + b[2];
  temp[1] = a[0]*b[3] + a[1]*b[4] + b[5];
  return temp;
}

float* randHomography(float* h){
	float scale = 0.00001;
	float * temp = new float[9]
	srand (time(NULL));
	float offset;
	for(int i=0;i<9;++i){
		offset = (rand() % 200 - 100) * scale;
		h[i] += offset;
	}
	return temp;
}

#endif
