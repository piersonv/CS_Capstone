// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"eriolHeader.h"
#include"../homography.h"
#include"NCCDemo.h"
#include"time.h"
#include<cstdlib>
Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

int main(int argc, char **argv)
{
  string tile = argv[1];
  string image = argv[2];
  string imageR = image+"R";
  string imageL = image+"L";
  double point[2];
  vector<Color> intcolors;
  vector<Color> intcolors2;
  double * current = new double[9];
  double * best = new double[9];
  float ncc;
  float bestncc = -2;
  float first;
  double scale = 0.01;
  bool initial = true;

  vector<PixelLoc> interiorR = getContour(tile, imageR);
  vector<PixelLoc> interiorL = getContour(tile, imageL);
  vector<Coord> fpSource;
  vector<Coord> fpDestination;
  vector<Coord> fpR = getFeaturePoints(tile, imageR);
  vector<Coord> fpL = getFeaturePoints(tile, imageL);
   Matrix3x3 myH1;
   vector<PixelLoc> interior;
   Image myimg;
   Image myimgOther;

  if (interiorR.size() > interiorL.size())
  {
    interior = getContour(tile, imageL);
    myH1  = getHomography(tile, imageL, imageR);
    //myH1  = getHomography(tile, imageR, imageL);
    myimg = imageR.c_str();
    myimgOther = imageL.c_str();
    fpSource = getFeaturePoints(tile, imageL); 
    fpDestination = getFeaturePoints(tile, imageR); 
  }
  else
  {
    interior = getContour(tile, imageR);
    myH1  = getHomography(tile, imageR, imageL);
   // myH1  = getHomography(tile, imageL, imageR);
    myimg = imageL.c_str();
    myimgOther = imageR.c_str();
    fpSource = getFeaturePoints(tile, imageR);
    fpDestination = getFeaturePoints(tile, imageL);

  }

  cerr << "homography: " << myH1 << endl; 
  for(int i=0;i<9;++i){
	current[i] = myH1.m[i];
  }

  cout << "Origonal Feature points: " ;
  for(unsigned int i=0;i<fpDestination.size();++i){
        cout << fpDestination[i] << " ";
  }

Color red(255,0,0);
Color blue(0,0,100);
Color black(0,0,0);
Color white(255,255,255);

Image imgInitial = myimg;
Image src = myimgOther;
for(unsigned int i=0; i<interior.size(); ++i){
   homography(interior[i].x, interior[i].y, current, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
       imgInitial.setPixel(loc,blue);
   }
}
for(unsigned int i=0;i<fpDestination.size();++i){
   homography(fpSource[i].x,fpSource[i].y, current, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
        imgInitial.setPixel(loc,red);
   }
}
imgInitial.print("initial.ppm");
for(unsigned int i=0; i<interior.size(); ++i){
   if(inImage(&src,interior[i])){
      src.setPixel(interior[i],blue);
   }
}
for(unsigned int i=0;i<fpDestination.size();++i){
   PixelLoc loc((int)fpSource[i].x, (int)fpSource[i].y);
   if(inImage(&src,loc)){ 
       src.setPixel(loc,red);
   }
}
src.print("src.ppm");

for(int k=10; k < 100000; k*=10){
  cout << endl << "Trying with scale = " << scale/k << endl;
  for(int j=0; j<20000; ++j){
  	for(unsigned int i=0; i<interior.size(); ++i){
		homography(interior[i].x + 0.5 , interior[i].y + 0.5, current, point);
		Coord mycoord(point[0], point[1]);
	        if(inImage(&myimg,mycoord)){	
	            intcolors.push_back(asInterpolatedColor(mycoord, &myimg));
		} else {
	            intcolors.push_back(black);
		    intcolors2.push_back(white);
	 	    continue;
		}
		intcolors2.push_back(myimgOther.getPixel(interior[i]));
 	 }
 	 ncc = calculate_normalized_correlation(intcolors, intcolors2);
 	 if (initial){
		first = ncc;
 	  	initial = false;
		//cout << "Initial: " << first << endl;
	 }
	 if (ncc > bestncc){
		//cout << "Found better: " << ncc << endl;
		bestncc = ncc;
		for(int i=0;i<9;++i){
			best[i] = current[i];
		}
		j=0;
	 }
	 long seed = (long)time(NULL) * j;
	 //cout << seed << endl;
         randHomography(best, current, seed, scale/k);
 	 intcolors.clear();
 	 intcolors2.clear();
         }
// cout << "Best so far: " << bestncc << endl;
// cout << "homography: ";
// for(int i=0;i<9;++i){
//        cout << best[i] << " ";
// }
// cout << endl;
 }
 cout << "First: " << first << " Best: " << bestncc << endl;
 cout << "homography: "; 
 for(int i=0;i<9;++i){
	cout << best[i] << " ";
 } 
 cout << endl;
 cout << "Origonal Feature points: " ;
 for(unsigned int i=0;i<fpDestination.size();++i){
	cout << fpDestination[i] << " ";
 }
 cout << endl << "Optimized Feature points: ";
 for(unsigned int i=0;i<fpDestination.size();++i){
      homography(fpSource[i].x,fpSource[i].y, best, point);
      cout << point[0] << "," << point[1] << " ";
 } 

Image imgFinal = myimg;

for(unsigned int i=0; i<interior.size(); ++i){
   homography(interior[i].x, interior[i].y, best, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
      imgFinal.setPixel(loc,blue);
   }
}
for(unsigned int i=0;i<fpDestination.size();++i){
   homography(fpSource[i].x,fpSource[i].y, best, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
      imgFinal.setPixel(loc,red);
   }
}
imgFinal.print("final.ppm");
system("/home/mscs/bin/show src.ppm initial.ppm final.ppm");
}
