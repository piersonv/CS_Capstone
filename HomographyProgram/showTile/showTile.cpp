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

  Matrix3x3 myH1 = getHomography(tile, imageR, imageL);
  cerr << "homography: " << myH1 << endl; 
  for(int i=0;i<9;++i){
	current[i] = myH1.m[i];
  }  
  Image myimg(imageL.c_str());
  Image myimgR(imageR.c_str());
  vector<PixelLoc> interiorL = getContour(tile, imageR);

  for(int k=10; k < 100000; k*=10){
  cout << "Trying with scale = " << scale/k << endl;
  for(int j=0; j<20000; ++j){
  	for(unsigned int i=0; i<interiorL.size(); ++i){
		homography(interiorL[i].x + 0.5 , interiorL[i].y + 0.5, current, point);
		Coord mycoord(point[0], point[1]);
		if (point[0] < myimg.getWidth() && point[1] < myimg.getHeight()){
	            intcolors.push_back(asInterpolatedColor(mycoord, &myimg));
		} else {
			continue;
		}
		intcolors2.push_back(myimgR.getPixel(interiorL[i]));
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
 
}
