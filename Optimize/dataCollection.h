#include <iostream>
#include <string>
#include <vector>
#include"time.h"
using namespace std;

class dataCollection
{
private:
	double * initialHomography, * finalHomography;
	string smallImage, imageR, imageL;
	string tile;
	double initialNCC, finalNCC, glareNCC;
	int version;

	void checkFile(string condition, int index, vector<dataCollection> entries);
	void checkFile(string condition1, string condition2, int initialIndex, vector<dataCollection> entries);

public:
	dataCollection(int v, string t, string sI, string iR, string iL, double iNCC, double fNCC, double gNCC, double * iH, double * fH);
	dataCollection(char ** entry);
	~dataCollection() {}

	double * getInitialHomography() { return initialHomography; }
	void setInitialHomography(double * iH ) { for (int i = 0; i<9; i++) initialHomography[i] = iH[i];}

	double * getFinalHomography() { return finalHomography; }
	void setFinalHomography(double * fH) { for (int i = 0; i<9; i++) finalHomography[i] = fH[i]; }

	string getImageRight() { return imageR; }
	void setImageRight(string iR) { imageR = iR; }

	string getImageLeft() { return imageL; }
	void setImageLeft(string iL) { imageL = iL; }

	string getSmallImage() { return smallImage; }
	void setSmallImage(string sI) { smallImage = sI; }

	double getInitialNCC() { return initialNCC; }
	void setInitialNCC(double iNCC) { initialNCC = iNCC; }

	double getFinalNCC() { return finalNCC; }
	void setFinalNCC(double fNCC) { finalNCC = fNCC; }

	int getGlareNCC() { return glareNCC; }
	void setGlareNCC(float gNCC) { glareNCC = gNCC; }

	void writeEntry(); //write entry to log file.

	void print(); //print object to terminal

	static vector<dataCollection> checkFile(string condition, int index);
	static vector<dataCollection> checkFile(string condition1, string condition2, int index);

	static vector<dataCollection> getAll(); //Returns a vector of every entry
	static std::vector<dataCollection> getEntriesByVersion(int v); //Returns vector of entries by version
	static vector<dataCollection> getEntriesByImage(string i); //Returns entries associated with given image.
	static vector<dataCollection> getEntriesByImagePair(string i1, string i2); //Returns entries associated with given image pair.
	static vector<dataCollection> getEntriesByTile(string t); //Returns entries associated with given tile
	static vector<dataCollection> getEntriesByNCCRange(float iNCC, float fNCC); //Returns entries between given range of NCC values.
	static vector<dataCollection> getEntriesByGlareNCC(float gNCC);
};