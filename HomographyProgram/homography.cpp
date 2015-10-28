#include <iostream>
#include "homography.h"
#include "eriol.h"

void printpoint(float* a){
  std::cout << a[0] << " " << a[1] << std::endl;
}

void printhomography(float* a){
  std::cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] << " " << a[5] << std::endl;
}

void testHomography(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
{
  vector<Coord> p, q;
  p.push_back(Coord(a,b)); q.push_back(Coord(g,h));
  p.push_back(Coord(c,d)); q.push_back(Coord(i,j));
  p.push_back(Coord(e,f)); q.push_back(Coord(k,l));
  p.push_back(Coord(0,-1)); q.push_back(Coord(1,0));

  Matrix3x3 hom = computeHomography(p,q);
  cout << "H: " << hom << endl;
  cout << hom*Coord(1,0) << "  " << hom*Coord(0,1) << endl;
  cout << hom*Coord(1,1) << "  " << hom*Coord(2,2) << endl;
}

int main(){
  float myP[2] = {1,2};
  std::cout<<"Input x: ";
  std::cin >> myP[0];
  std::cout<<"Input y: ";
  std::cin >> myP[1];
  float myH[6] = {1,0,1,0,1,1};
  std::cout<<"First point: ";
  printpoint(myP);
  std::cout<<"Point after traslation: ";
  printpoint(homography(myP, myH));
  std::cout<<"\n";
  return -1;

}
