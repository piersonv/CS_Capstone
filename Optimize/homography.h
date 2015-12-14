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


#endif
