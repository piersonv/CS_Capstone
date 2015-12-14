// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"time.h"
#include"optimize.h"
#include"glareReduction.h" 
#include"dataCollection.h"
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
  double * current = new double[9];
  double * best = new double[9];
  double * init = new double[9];
  double ncc;
  double bestncc = -2;
  double first;
  double scale = 0.01;
  bool optimize = true;

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
  if(argc == 12){
	optimize = false;
	for(int i=0;i<9;++i){
        	current[i] = best[i] = (double)atof(argv[3+i]);
	}
  	current[8] = best[8] = 1;	
  }else if(argc == 21){
	optimize = false;
	cout << "Comparing" << endl;
       	for(int i=0;i<18;++i){
                if (i < 9){
			current[i] = (double)atof(argv[3+i]);
		} else {
			best[i-9] = (double)atof(argv[3+i]);
		} 
        }
        current[8] = best[8] = 1;
  }else{
  	cerr << "homography: " << myH1 << endl; 
  	for(int i=0;i<9;++i){
		init[i] = current[i] = best[i] = myH1.m[i];
  	}
  	init[8] = current[8] = best[8] = 1;
  }
  cout << "Original Feature points: " ;
  for(unsigned int i=0;i<fpDestination.size();++i){
        cout << fpDestination[i] << " ";
  }

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
for(unsigned int i=0;i<fpDestination.size();++i){
   homography(fpSource[i].x,fpSource[i].y, current, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
        imgInitial.setPixel(loc,red);
   }
}
string imagename = "TileImages/" + tile + "_initial.ppm";
imgInitial.print(imagename.c_str());
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
cout << endl;

if(optimize){
        Optimize (scale, first, ncc, bestncc, &interior, init, current, best, &myimg, &myimgOther);
}

Image imageWithGlare = myimg;

 int ncc_glare_reduced = calculateNCCWithoutGlare(&interior, best, &myimg, &myimgOther, &imageWithGlare);
 cout << "First: " << first << " Best: " << bestncc <<  "Glare Reduced: " << ncc_glare_reduced << endl;
 cout << "homography: "; 
 for(int i=0;i<9;++i){
	cout << current[i] << " ";
 } 
 cout << endl;
 
 cout << endl << "Optimized Feature points: ";
 for(unsigned int i=0;i<fpDestination.size();++i){
      homography(fpSource[i].x,fpSource[i].y, best, point);
      cout << point[0] << "," << point[1] << " ";
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
for(unsigned int i=0;i<fpDestination.size();++i){
   homography(fpSource[i].x,fpSource[i].y, best, point);
   PixelLoc loc((int)point[0], (int)point[1]);
   if(inImage(&imgInitial,loc)){
      imgFinal.setPixel(loc,red);
   }
}

dataCollection entry(1, tile, myimg, imageR, imageL, first, best, ncc_glare_reduced, myH1.m, best);
entry.writeEntry();

imagename = "TileImages/" + tile + "_final.ppm";
imgFinal.print(imagename.c_str());
imagename = "TileImages/" + tile + "_glare.ppm";
imageWithGlare.print(imagename.c_str());
system("/home/mscs/bin/show src.ppm initial.ppm final.ppm glare.ppm");

}
