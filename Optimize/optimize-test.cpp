// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"eriolHeader.h"
#include"optimize.h"
#include"NCCDemo.h"
#include"time.h"
#include<cstdlib>

int main(int argc, char **argv)
{
  cout << "Starting" << endl;
  double point[2];
  double * current = new double[9];
  double * best = new double[9];
  double * init = new double[9];
  double ncc;
  double bestncc = -2;
  double first;
  double scale = 1;
  //int position = 0;
  //int direction = 1;
  bool optimize = true;

 vector<PixelLoc> interior;
 for(int i=9; i<=23; ++i){
	for(int j=9; j<=23; ++j){
	PixelLoc point(i, j);
	interior.push_back(point);
	}
 }
 Image myimg("test-initial.ppm");
 Image myimgOther("test-final.ppm");
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

cout << endl;

if(optimize){
	Optimize (scale, first, ncc, bestncc, &interior, init, current, best, &myimg, &myimgOther);
}
 
 cout << "First: " << first << " Best: " << bestncc << endl;
 cout << "homography: "; 
 for(int i=0;i<9;++i){
	cout << current[i] << " ";
 } 
 cout << endl;
Image imgFinal = myimg;

printHomographyTile(&imgFinal,&imgInitial,interior,best);
system("/home/mscs/bin/show src.ppm initial.ppm final.ppm");
}
