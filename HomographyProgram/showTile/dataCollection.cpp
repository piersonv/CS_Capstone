#include "dataCollection.h"
#include "parser.h"
#include <fstream> 
#include <iostream>
#include <stdio.h>
using namespace std;


dataCollection::dataCollection(string t, string sI, string iR, string iL, int iNCC, int fNCC, double * iH, double * fH)
{
	tile = t;
	smallImage = sI;
	imageR = iR;
	imageL = iL;
	initialNCC = iNCC;	
	finalNCC = fNCC;
	initialHomography = new double[9];
	finalHomography = new double[9];
	for (int i = 0; i < 9; i++)
	{
		initialHomography[i] = iH[0];
		finalHomography[i] = fH[0];
	}
}

void dataCollection::writeEntry() //write entry to log file.
{
	ofstream stream;
	stream.open("tileData.txt", std::ofstream::out | std::ofstream::app);

	stream << tile << " ";
	stream << smallImage << " ";
	stream << imageR << " " << imageL << " ";
	stream << initialNCC << " " << finalNCC << " ";
	//stream << initialHomography << " " << finalHomography << "\n";
	for (int i = 0; i < (signed)sizeof(initialHomography); i++)
	{
		stream << initialHomography[i] << " ";
	}

	for (int i = 0; i < (signed)sizeof(finalHomography); i++)
	{
		stream << finalHomography[i] << " ";
	}

	stream << endl;

	stream.close();
}

void dataCollection::print() //print object to terminal
{
	cout << tile << " | ";
	cout << smallImage << " | ";
	cout << imageR << " | " << imageL << " | ";
	cout << initialNCC << " | " << finalNCC << " | ";
	//cout << initialHomography << " " << finalHomography << "\n";
	for (int i = 0; i < (signed)sizeof(initialHomography); i++)
	{
		cout << initialHomography[i] << " | ";
	}

	for (int i = 0; i < (signed)sizeof(finalHomography); i++)
	{
		cout << finalHomography[i] << " | ";
	}

	cout << endl;
}

/*void checkFile(string condition, int index, vector<dataCollection> entries)
{
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];

	int numLines = 0;
	ifstream in("test.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("test.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		parser(parsed_line, line);
		cout << "Got here" << endl;
		for (int i = 0; i < (signed)sizeof(parsed_line); i++)
		{
			cout << parsed_line[i] << endl;
		}
		entry_count++;
	}

	stream.close();
}

void checkFile(string condition1, string condition2, int initialIndex, vector<dataCollection> entries)
{
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];

	int numLines = 0;
	ifstream in("test.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("test.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		parser(parsed_line, line);
		cout << "Got here" << endl;
		for (int i = 0; i < (signed)sizeof(parsed_line); i++)
		{
			cout << parsed_line[i] << endl;
		}
		entry_count++;
	}

	stream.close();
}

vector<dataCollection> dataCollection::getAll() //Static. Returns a vector of every entry
{
	vector<dataCollection> entries;
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];

	int numLines = 0;
	ifstream in("test.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("test.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		parser(parsed_line, line);
		cout << "Got here" << endl;
		for (int i = 0; i < (signed)sizeof(parsed_line); i++)
		{
			cout << parsed_line[i] << endl;
		}
		entry_count++;
	}

	stream.close();	
	return entries;
}

vector<dataCollection> dataCollection::getEntriesByImage(string i) //Static. Returns entries associated with given image.
{
	vector<dataCollection> entries;
	checkFile(i, 2, entries);
	return entries;
}

vector<dataCollection> dataCollection::getEntriesByImagePair(string i1, string i2) //Static. Returns entries associated with given image pair.
{
	vector<dataCollection> entries;
	checkFile(i1, i2, 2, entries);
	return entries;
}

vector<dataCollection> dataCollection::getEntriesByTile(string t) //Static. Returns entries associated with given tile
{
	vector<dataCollection> entries;
	checkFile(t, 0, entries);
	return entries;
}

vector<dataCollection> dataCollection::getEntriesByNCCRange(int iNCC, int fNCC) //Static. Returns entries between given range of NCC values.
{
	vector<dataCollection> entries;
	checkFile(to_string(iNCC), to_string(fNCC), 4, entries);
	return entries;
}*/