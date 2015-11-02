#include <iostream>
#include "homography.h"
//#include "eriolHeader.h"

void printpoint(float* a){
  std::cout << a[0] << " " << a[1] << std::endl;
}

void printhomography(float* a){
  std::cout << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] << " " << a[5] << std::endl;
}


int main(){
  float myP[2] = {1,2};
  std::cout<<"Input x: ";
  std::cin >> myP[0];
  std::cout<<"Input y: ";
  std::cin >> myP[1];
  float myH[9] = {1,0,1,0,1,1,1,1,1};
  std::cout<<"First point: ";
  printpoint(myP);
  std::cout<<"Point after traslation: ";
  printpoint(homography(myP, myH));
  
  printhomography(randHomography(myH));
  printpoint(homography(myP, randHomography(myH)));

  std::cout<<"\n";
  return -1;

}
