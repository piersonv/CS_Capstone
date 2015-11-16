// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"eriolHeader.h"
#include"../homography.h"
#include"NCCDemo.h"
#include"time.h"
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
  double scale = 1;
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
    myimg = imageR.c_str();
    myimgOther = imageL.c_str();
    fpSource = getFeaturePoints(tile, imageL); 
    fpDestination = getFeaturePoints(tile, imageR); 
  }
  else
  {
    interior = getContour(tile, imageR);
     myH1  = getHomography(tile, imageR, imageL);
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
  cout << endl << "Optimized Feature points: ";
  for(unsigned int i=0;i<fpDestination.size();++i){
      homography(fpSource[i].x,fpSource[i].y, current, point);
      cout << point[0] << "," << point[1] << " ";
  }

Color red(255,0,0);
Image imgInitial = myimgOther;
for(unsigned int i=0; i<interior.size(); ++i){
   imgInitial.setPixel(interior[i],red);
}
imgInitial.print("initial.ppm");

for(int k=10; k < 100000; k*=10){
  cout << "Trying with scale = " << scale/k << endl;
  for(int j=0; j<20000; ++j){
  	for(unsigned int i=0; i<interior.size(); ++i){
		homography(interior[i].x + 0.5 , interior[i].y + 0.5, current, point);
		Coord mycoord(point[0], point[1]);
		if (point[0] < myimg.getWidth() && point[1] < myimg.getHeight()){
	            intcolors.push_back(asInterpolatedColor(mycoord, &myimg));
		} else {
			continue;
		}
		intcolors2.push_back(myimgOther.getPixel(interior[i]));
 	 }
 	 ncc = calculate_normalized_correlation(intcolors, intcolors2);
 	 if (initial){
		first = ncc;
 	  	initial = false;
		cout << "Initial: " << first << endl;
	 }
//	 if (j%22 == 0)
//		cerr << ncc << endl;
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
 cout << "Best so far: " << bestncc << endl;
 cout << "homography: ";
 for(int i=0;i<9;++i){
        cout << best[i] << " ";
 }
 cout << endl;
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
for(unsigned int i=0; i<interior.size(); ++i){
        homography(interior[i].x + 0.5 , interior[i].y + 0.5, best, point);
        Coord mycoord(point[0], point[1]);
        if (point[0] < myimg.getWidth() && point[1] < myimg.getHeight()){
           myimgOther.setPixel(interior[i],asInterpolatedColor(mycoord, &myimg));
        } else {
                 continue;
        }
}
myimgOther.print("initial.ppm");

}
