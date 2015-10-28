// by Olaf Hall-Holt, 2007-2015
#include"eriolHeader.h"
Tile &loadJustOneTile(const string &tileID, const string &imgName);
vector<PixelLoc> getPixelsFor(int);

int main(int argc, char **argv)
{
  // get superpixel information
  Tile &t = loadJustOneTile("135leesn", "1098L");
  vector<int> sp = t.getSuperPixels("1098L");  // get the superpixels
  for ( unsigned i=0,i_end=sp.size(); i<i_end; ++i )
    vector<PixelLoc> in = getPixelsFor(sp[i]);
  // get locations of all pixels in a contour
  vector<PixelLoc> interior = getContour("135leesn", "1098L");
  cerr << "interior.size() " << interior.size() << " interior[0] " << interior[0] << " interior.back() " << interior.back() << endl;
  // get raw boundary locations
  vector< vector<Coord> > boundary = getContourBoundary("135leesn", "1098L");
  cerr << "boundary.size() " << boundary.size() << " boundary[0][0] " << boundary[0][0] << endl;
  // get the colors of (almost all) pixels in a contour
  Image img = getContourColors("134leesn", "1098L");
  img.print("out.ppm");
  // get feature points associated with a given tile
  vector<Coord> pt = getFeaturePoints("135leesn", "1098L");
  cerr << "feature points: " << pt << endl;
  // get homography between two contours of a tile
  cerr << "homography: " << getHomography("100leesn", "1098L", "1098R") << endl;
}
