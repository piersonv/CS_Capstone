// by Olaf Hall-Holt, 2007-2015
#include<iostream>
#include<string>
#include<cstdlib>
#include"eriolHeader.h"

int main(int argc, char **argv)
{
  string image = argv[1];
  string imageR = image+"R";
  string imageL = image+"L";
  vector<string> tiles = getTileIDsForImage(imageR);
  
  for(int i=0;i<tiles.size();++i){
	string path = "/home/users/npa/team-two-two/Optimize/Optimize " + tiles[i] + " " + image; 
  	cout << "Running " << tiles[i] << endl;
  	system(path.c_str());
  }
//system(imagepaths.c_str());
}
