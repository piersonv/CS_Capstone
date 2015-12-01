// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include"time.h"
#include"glareReduction.h" 
#include<cstdlib>
Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

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
  string tile = argv[1];
  string image = argv[2];
  string imageR = image+"R";
  string imageL = image+"L";
  double point[2];
  double * current = new double[9];
  double * best = new double[9];
  float ncc;
  float bestncc = -2;
  float first;
  double scale = 0.001;
  bool initial = true;
  int position = 0;
  int direction = 1;
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
//  }else if(argc == 13){
//	optimize = true;
//       for(int i=0;i<9;++i){
//                current[i] = best[i] = (double)atof(argv[3+i]);
//        }
//        current[8] = best[8] = 1;
  }else{
  	cerr << "homography: " << myH1 << endl; 
  	for(int i=0;i<9;++i){
		current[i] = best[i] = myH1.m[i];
  	}
  	current[8] = best[8] = 1;
  }
  cout << "Origonal Feature points: " ;
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
if(optimize){
for(int p=0;p<4;++p){
 for(int i=10; i<=100000000; i*=10){
 	cout <<"\nScale = " << scale/i << endl;
   for(int j=0; j<50; ++j){
  	ncc = calcNCC(&interior, current, &myimg, &myimgOther);
	if (initial){
		first = ncc;
   		initial = false;
		//cout << "Initial: " << first << endl;
	}
	if (ncc > bestncc){
		//cerr << ".";
                bestncc = ncc;
		j=0;
		best[position] = current[position];
		best[(position+1)%7] = current[(position+1)%7];
		best[(position-1)%7] = current[(position-1)%7];
		
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
  }
 }
}
// cout << "Best so far: " << bestncc << endl;
// cout << "homography: ";
// for(int i=0;i<9;++i){
//        cout << best[i] << " ";
// }
// cout << endl;

Image imageWithGlare = myimg;


 int ncc_glare_reduced = calculateNCCWithoutGlare(&interior, best, &myimg, &myimgOther, &imageWithGlare);
 cout << "First: " << first << " Best: " << bestncc <<  "Glare Reduced: " << ncc_glare_reduced << endl;
 cout << "homography: "; 
 for(int i=0;i<9;++i){
	cout << current[i] << " ";
 } 
 cout << endl;
// cout << "Origonal Feature points: " ;
// for(unsigned int i=0;i<fpDestination.size();++i){
//	cout << fpDestination[i] << " ";
// }
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
imgFinal.print("final.ppm");
imageWithGlare.print("glare.ppm");
system("/home/mscs/bin/show src.ppm initial.ppm final.ppm glare.ppm");
}
