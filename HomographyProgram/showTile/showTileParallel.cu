// by Olaf Hall-Holt, 2007-2015
#include <iostream>
#include <string>
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include "eriolHeader.h"
#include "../homography.h"
#include "NCCDemo.h"
#include "time.h"

Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

//Global variables
string tile;
string image;
string imageR;
string imageL;
double * best = new double[9];
float bestncc = -2;
float first;
double scale = 1;
bool initial = true;
thrust::host_vector<PixelLoc> interiorR;
thrust::host_vector<PixelLoc> interiorL;
thrust::host_vector<PixelLoc> interior;
Image myimg;
Image myimgOther;

void runNCC(double * current, int j, int k){
  //main variables
  double point[2];
  thrust::device_vector<Color> d_intcolors = intcolors;
  thrust::device_vector<Color> d_intcolors2 = intcolors2;
  float ncc;
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

   if (ncc > bestncc){
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

int main(int argc, char **argv)
{
  //main variables
  // double point[2];
  // /*thrust::host_*/vector<Color> intcolors;
  // /*thrust::host_*/vector<Color> intcolors2;
  double * current = new double[9];
  // float ncc;
  Matrix3x3 myH1;

  // //GPU variables
  // double * d_point;
  // float d_ncc; //in function
  // Matrix3x3 d_myH1;

  tile = argv[1];
  image = argv[2];
  imageR = image+"R";
  imageL = image+"L";
  interiorR = getContour(tile, imageR);
  interiorL = getContour(tile, imageL);

  best = new double[9];

  if (interiorR.size() > interiorL.size())
  {
    interior = getContour(tile, imageL);
    myH1  = getHomography(tile, imageL, imageR);
    myimg = imageR.c_str();
    myimgOther = imageL.c_str();
  }
  else
  {
    interior = getContour(tile, imageR);
    myH1  = getHomography(tile, imageR, imageL);
    myimg = imageL.c_str();
    myimgOther = imageR.c_str();
  }

  cerr << "homography: " << myH1 << endl; 
  for(int i=0;i<9;++i){
  current[i] = myH1.m[i];
  } 

  // //CUDA allocation
  // cudaMalloc(d_point, 2*sizeof(double));
  // cudaMalloc(d_current, 9*sizeof(double));

  // //CUDA copying
  

  for(int k=10; k < 100000; k*=10){
  cout << "Trying with scale = " << scale/k << endl;

  float d_bestncc = -2;
  double * d_best;
    

  runNCC(current, j, k);


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
