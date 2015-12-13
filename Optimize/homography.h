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

double calcNCC(vector<PixelLoc> *interior, double * current, Image *myimg, Image *myimgOther)
{
        Color black(0,0,0);
        Color white(255,255,255);
        vector<Color> signal1,signal2;
        double point[2];
        for(unsigned int i=0; i<interior->size(); ++i){
                homography(interior[0][i].x + 0.5 , interior[0][i].y + 0.5, current, point);
                Coord mycoord(point[0], point[1]);
                if(inImage(myimg,mycoord)){
                    signal1.push_back(asInterpolatedColor(mycoord, myimg));
                } else {
                    signal1.push_back(black);
                    signal2.push_back(white);
                    continue;
                }
                signal2.push_back(myimgOther->getPixel(interior[0][i]));
         }
         return calculate_normalized_correlation(signal1, signal2);
}


void printHomographyTile(Image *imgFinal, Image *imgInitial, vector<PixelLoc> interior, double *best){
	double point[2];
        for(unsigned int i=0; i<interior.size(); ++i){
           homography(interior[i].x, interior[i].y, best, point);
           double intpartx, fracpartx, intparty, fracparty, intfracx, intfracy;
           fracpartx = modf(point[0], &intpartx);
           fracparty = modf(point[1], &intparty);
           intfracx = 1-fracpartx;
           intfracy = 1-fracparty;
           Color col1(0,0,255*(intfracx*intfracy));
           Color col2(0,0,255*(fracpartx*intfracy));
           Color col3(0,0,255*(fracparty*intfracx));
           Color col4(0,0,255*(fracpartx*fracparty));
           PixelLoc loc(intpartx, intparty);
           PixelLoc loc2(intpartx+1, intparty);
           PixelLoc loc3(intpartx, intparty+1);
           PixelLoc loc4(intpartx+1, intparty+1);

           if(inImage(imgInitial,loc)){
                imgFinal->setPixel(loc,imgFinal->getPixel(loc)+col1);
           }
           if(inImage(imgInitial,loc2)){
                imgFinal->setPixel(loc2,imgFinal->getPixel(loc2)+col2);
           }
           if(inImage(imgInitial,loc3)){
                imgFinal->setPixel(loc3,imgFinal->getPixel(loc3)+col3);
           }
           if(inImage(imgInitial,loc4)){
                imgFinal->setPixel(loc4,imgFinal->getPixel(loc4)+col4);
           }

	}
imgFinal->print("final.ppm");
}


void Optimize(double scale, double &first, double &ncc, double &bestncc, vector<PixelLoc> *interior, double * init, double * current, double * best, Image *myimg, Image *myimgOther){
	bool initial = true;
	bool success = true;
	int i=1;
	int failures = 0;
        while(failures<2){
		success = false;
                int count = 0;
		i*=10;
                cout <<"Scale = " << (scale/i) << endl;
                double offset = -50*(scale/i);
                for(int l=0; l<2; ++l){
                        for(int k=0; k < 8; ++k){
                                for(int j=1; j<=100; ++j){
                                        ncc = calcNCC(interior, current, myimg, myimgOther);
                                        if (initial){
                                                first = ncc;
                                                initial = false;
                                        }
                                        if (ncc > bestncc){
                                                l=0;
						success=true;
                                                ++count;
                                                bestncc = ncc;
                                                best[k] = current[k];
                                        }
                                randHomography(k, init, current, offset + (scale/i)*j);
                                }
                                for(int j=0; j<9; ++j){
                                        init[j] = current[j] =  best[j];
                                }
                        }
                        cout << "." << endl;
                }
                cout << "\nhomography: ";
                for(int i=0;i<9;++i){
                        cout << current[i] << " ";
                }
                cout << endl;
		if(!success){
			++failures;
		}
		//printHomographyTile(myimg, myimg, *interior, best);
		//system("/home/mscs/bin/show final.ppm"); 
       }
}
#endif
