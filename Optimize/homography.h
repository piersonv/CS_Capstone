#ifndef _HOMOGRAPHY_
#define _HOMOGRAPHY_

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "eriolHeader.h"
#include "NCCDemo.h"
// homography(x coord, y coord, homography, point)
void homography(double x, double y, double* b, double *c){
  double divide = x*b[6] + y*b[7] + b[8];
  c[0] = (x*b[0] + y*b[1] + b[2]) / divide;
  c[1] = (x*b[3] + y*b[4] + b[5]) / divide;
}

bool inverse_homography(double* inverse, double* b){
  double cofacs[9];
  cofacs[0] = ((b[4]*b[8])-(b[5]*b[7]));
  cofacs[1] = -1*((b[3]*b[8])-(b[5]*b[6]));
  cofacs[2] = ((b[3]*b[7])-(b[4]*b[6]));
  cofacs[3] = -1*((b[1]*b[8])-(b[2]*b[7]));
  cofacs[4] = ((b[0]*b[8])-(b[2]*b[6]));
  cofacs[5] = -1*((b[0]*b[7])-(b[1]*b[6]));
  cofacs[6] = ((b[1]*b[4])-(b[2]*b[5]));
  cofacs[7] = -1*((b[0]*b[3])-(b[2]*b[5]));
  cofacs[8] = ((b[0]*b[4])-(b[1]*b[3]));
	
  double det = ((b[0]*cofacs[0])+(b[1]*cofacs[1])+(b[2]*cofacs[2]));
  if(det==0){
	return false;
  }else{
	det=1/det;
  }
  inverse[0] = det*cofacs[0];
  inverse[1] = det*cofacs[3];
  inverse[2] = det*cofacs[6];
  inverse[3] = det*cofacs[1];
  inverse[4] = det*cofacs[4];
  inverse[5] = det*cofacs[7];
  inverse[6] = det*cofacs[2];
  inverse[7] = det*cofacs[5];
  inverse[8] = det*cofacs[8];
  return true;
}



//randHomography(base homography, result homography, rand seed, scale)
void randHomography(double* h, double* temp, long now, double scale=0.00001){
        srand (now);
        double offset;
        for(int i=0;i<9;++i){
                offset = (rand() % 200 - 100) * scale;
                temp[i] = h[i] + offset;
        }

}

void randHomography(int position, double* k, double* h, double scale){
	h[position] = k[position] + (scale);
}

void randHomography(int direction, int position, double* k, double* h, double scale){
	h[position] = k[position] + (direction*scale);
}

#endif
