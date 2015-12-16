#ifndef OPTIMIZE
#define OPTIMIZE

#include <iostream>
#include "homography.h"
#include "ncc_CUDA.h"

#ifndef CUDA
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
            signal2.push_back(myimgOther->getPixel(interior[0][i]));
        } else {
            signal1.push_back(black);
            signal2.push_back(white);
        }
        cout << signal1[i] << " " << signal2[i] << " ";
    }
    return calculate_normalized_correlation(signal1, signal2);
}


double calcNCCInv(vector<PixelLoc> *interior, double * current, Image *myimg, Image *myimgOther)
{
    Color black(0,0,0);
    Color white(255,255,255);
    vector<Color> signal1,signal2;
    double point[2];
    double inverse[9];
    inverse_homography(inverse, current);
    for(unsigned int i=0; i<interior->size(); ++i){
        homography(interior[0][i].x + 0.5 , interior[0][i].y + 0.5, inverse, point);
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

#endif

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
        }else{
	    failures=0;
	}
        //printHomographyTile(myimg, myimg, *interior, best);
        //system("/home/mscs/bin/show final.ppm"); 
    }
}


void Optimize(double scale, double &first, double &ncc, double &bestncc, vector<PixelLoc> *interior, vector<PixelLoc> *interiorOther, double * init, double * current, double * best, Image *myimg, Image *myimgOther){
    bool initial = true;
    bool success = true;
    double ncc1;
    double ncc2;
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
                    ncc1 = calcNCC(interior, current, myimg, myimgOther);
                    ncc2 = calcNCCInv(interiorOther, current, myimgOther, myimg);
		    ncc = (ncc1+ncc2)/2;
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
    }else{
    failures=0;
	}
        //printHomographyTile(myimg, myimg, *interior, best);
        //system("/home/mscs/bin/show final.ppm"); 
    }
}

void Optimize(double scale, double &first, double &ncc, double &bestncc, vector<PixelLoc> *interior, double * current, double * best, Image *myimg, Image *myimgOther){
  bool initial = true;
  bool success = true;
  int position = 0;
  int direction = 0;
  int i=1;
  int failures = 0;
  while(failures<2){
    success = false;
    i*=10;
    int count=0;
    //cout <<"Scale = " << (scale/i) << endl;
    for(int l=0; l<2; ++l){
      for(int j=0; j<50; ++j){
	ncc = calcNCC(interior, current, myimg, myimgOther);
	if (initial){
	  first = ncc;
	  initial = false;
	}
	if ((ncc-bestncc)>(scale/(i*10))){
	  l=0;
	  ++count;
	  success=true;
	  bestncc = ncc;
	  best[position] = current[position];
	  best[(position+1)%7] = current[(position+1)%7];
	  best[(position-1)%7] = current[(position-1)%7];
          //cout << "." << flush;	  
	}else{
	  if(direction == -1){
	    direction = 1;
	    if (position == 7){
	      position = 0;
	    }else{
	      ++position;
	    }
	  }else{
	    direction = -1;
	  }
	  
	}
	randHomography(direction, position, best, current, scale/i);
      	}
	//cout << bestncc << endl;
    }	
    if(!success){
      ++failures;
    }else{
      failures=0;
    }
 //   cout << "\nCount: " << count << endl;
  }
}
#endif
