// by Olaf Hall-Holt, 2007-2015
#include"eriolHeader.h"
#include"../homography.h"
#include"NCCDemo.h"
Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

int main(int argc, char **argv)
{
  // get superpixel information
  //Tile &t = loadJustOneTile("169npa3", "1094L");
  //vector<int> sp = t.getSuperPixels("1094L");  // get the superpixels
  //for ( unsigned i=0,i_end=sp.size(); i<i_end; ++i )
  //  vector<PixelLoc> in = getPixelsFor(sp[i]);
  // get locations of all pixels in a contour
  Matrix3x3 myH1 = getHomography("205leesn", "1098R", "1098L");
  //Matrix3x3 myH2 = getHomography("205leesn", "1098L", "1098R");
  //Matrix3x3 myH = getHomography("169npa3", "1194R", "1194L");
  cerr << "homography: " << myH1 << endl; 
  //cerr << "homography: " << myH2 << endl; 
  Image myimg("1098L");
  cerr << myimg.getHeight() << " " << myimg.getWidth() << endl;
  Image imgR = getContourColors("205leesn", "1098R");  
  vector<PixelLoc> interiorL = getContour("205leesn", "1098R");
  double *point;
  vector<Color> intcolors;
  vector<Color> intcolors2;
  for(unsigned int i=0; i<interiorL.size(); ++i){
	point = homography(interiorL[i].x + 0.5 , interiorL[i].y + 0.5, myH1.m);
	Coord mycoord(point[0], point[1]);
	PixelLoc myloc((int)point[0], (int)point[1]);
	if (point[0] < myimg.getWidth() && point[1] < myimg.getHeight()){
            //intcolors.push_back(asInterpolatedColor(mycoord, &myimg));
            intcolors.push_back(myimg.getPixel(myloc));
	} else {
		cerr << mycoord << endl;
		continue;
	}
	intcolors2.push_back(imgR.getPixel(interiorL[i]));
  }
 
  cout << calculate_normalized_correlation(intcolors, intcolors2) << endl; 
  //cerr << "interior.size() " << interior.size() << " interior[0] " << interior[0] << " interior.back() " << interior.back() << endl;
  // get raw boundary locations
  //vector< vector<Coord> > boundary = getContourBoundary("135leesn", "1098L");
  //cerr << "boundary.size() " << boundary.size() << " boundary[0][0] " << boundary[0][0] << endl;
  // get the colors of (almost all) pixels in a contour
  // Image imgR = getContourColors("205leesn", "1098R");
  //img.print("out.ppm");
  // get feature points associated with a given tile
  //vector<Coord> pt = getFeaturePoints("135leesn", "1098L");
  //cerr << "feature points: " << pt << endl;
  // get homography between two contours of a tile
}
