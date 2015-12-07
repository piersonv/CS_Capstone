// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"eriolHeader.h"
#include"homography.h"
#include"NCCDemo.h"
#include"time.h"
#include<cstdlib>

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


int main(int argc, char **argv)
{
  cout << "Starting" << endl;
  double point[2];
  double * current = new double[9];
  double * best = new double[9];
  double * init = new double[9];
  float ncc;
  float bestncc = -2;
  float first;
  double scale = 0.1;
  bool initial = true;
  //int position = 0;
  //int direction = 1;
  bool optimize = true;

 cout << "Starting" << endl; 
 vector<PixelLoc> interior;
 for(int i=5; i<=10; ++i){
	for(int j=5; j<=10; ++j){
	PixelLoc point(i, j);
	interior.push_back(point);
	}
 }
cout << "Creating Images" << endl;
 Image myimg("test-initial.ppm");
 Image myimgOther("test-final.ppm");
cout << "Images created" << endl;
 for(int i=0;i<9;++i){
		init[i] = current[i] = best[i] = 0; 
 }
  	init[8] = current[8] = best[8] = 1;
  	init[0] = current[0] = best[0] = 1;
  	init[4] = current[4] = best[4] = 1;
  

Color red(255,0,0);
Color blue(0,0,100);

Image imgInitial = myimg;
Image src = myimgOther;

for(unsigned int i=0; i<interior.size(); ++i){
   homography(interior[i].x, interior[i].y, current, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
       imgInitial.setPixel(loc,blue);
   }
}

imgInitial.print("initial.ppm");

for(unsigned int i=0; i<interior.size(); ++i){
   if(inImage(&src,interior[i])){
      src.setPixel(interior[i],blue);
   }
}
src.print("src.ppm");

int count;
cout << endl;
if(optimize){
	for(double i=1;i<=1000000;i*=10){
	count = 0;
 	cout <<"Scale = " << (scale/i) << endl;
		
	double offset = -50*(scale/i);
	for(int l=0; l<2; ++l){
	for(int k=0; k < 8; ++k){
		for(int j=1; j<=100; ++j){
 			ncc = calcNCC(&interior, current, &myimg, &myimgOther);
			if (initial){
				first = ncc;
   				initial = false;
			}
			if (ncc > bestncc){
				l=0;
	   	       		++count;
				bestncc = ncc;
				best[k] = current[k];
			}
     		randHomography(k, init, current, offset + (scale/i)*j);
    		}
		cout << "Count: " << count << endl;
		count = 0;
		for(int j=0; j<9; ++j){
			init[j] = current[j] =  best[j];
		}	
	}
	}
	
	}
}
 
 cout << "First: " << first << " Best: " << bestncc << endl;
 cout << "homography: "; 
 for(int i=0;i<9;++i){
	cout << current[i] << " ";
 } 
 cout << endl;
Image imgFinal = myimg;

for(unsigned int i=0; i<interior.size(); ++i){
   homography(interior[i].x, interior[i].y, best, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
      imgFinal.setPixel(loc,blue);
   }
}
imgFinal.print("final.ppm");
system("/home/mscs/bin/show src.ppm initial.ppm final.ppm");
}
