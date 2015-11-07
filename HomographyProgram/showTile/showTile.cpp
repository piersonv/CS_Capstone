// by Olaf Hall-Holt, 2007-2015
#include"eriolHeader.h"
#include"../homography.h"
#include"NCCDemo.h"
#include"time.h"
Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

int main(int argc, char **argv)
{
  Matrix3x3 myH1 = getHomography("205leesn", "1098R", "1098L");
  cerr << "homography: " << myH1 << endl; 
  Image myimg("1098L");
  Image myimgR("1098R");
  //cerr << myimg.getHeight() << " " << myimg.getWidth() << endl;
  Image imgR = getContourColors("205leesn", "1098R");  
  vector<PixelLoc> interiorL = getContour("205leesn", "1098R");
  //cerr << "got tile positions" << endl;
  double *point;
  vector<Color> intcolors;
  vector<Color> intcolors2;
  Matrix3x3 best = myH1;
  float bestncc = -2;
  float first;
for(int j=0; j<2000; ++j){
  for(unsigned int i=0; i<interiorL.size(); ++i){
	point = homography(interiorL[i].x + 0.5 , interiorL[i].y + 0.5, myH1.m);
	Coord mycoord(point[0], point[1]);
	PixelLoc myloc((int)point[0], (int)point[1]);
	//cerr << myloc << endl;
	if (point[0] < myimg.getWidth() && point[1] < myimg.getHeight()){
            intcolors.push_back(asInterpolatedColor(mycoord, &myimg));
            //intcolors.push_back(myimg.getPixel(myloc));
	} else {
		//cerr << mycoord << endl;
		continue;
	}
	intcolors2.push_back(myimgR.getPixel(interiorL[i]));
  }
  float ncc = calculate_normalized_correlation(intcolors, intcolors2);
  //cout << "NCC is: " << float << endl; 
  if (j==0){
	first = ncc;
  }
  //if (j%100 == 0) {
  //   cout << ncc << endl;
  //}
  if (ncc > bestncc){
	cout << "Found better: " << ncc << endl;
	bestncc = ncc;
	best = myH1;
	j=0;
  }
  double * temp = randHomography(best.m, time(NULL), 0.0000001);
  //myH1.m = randHomography(best.m, time(NULL), 0.0000001);
  for(int i=0;i<9;++i){
 	myH1.m[i] = temp[i];
  }
  delete temp;
  intcolors.clear();
  intcolors2.clear();
}
 cout << "First: " << first << " Best: " << bestncc << endl;
 cerr << "homography: " << best << endl; 
 
}
