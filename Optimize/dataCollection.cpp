#include "dataCollection.h"
#include "parser.h"
#include <fstream> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


dataCollection::dataCollection(int v, string t, string sI, string iR, string iL, double iNCC, double fNCC, double gNCC, double * iH, double * fH)
{
	version = v;
	tile = t;
	smallImage = sI;
	imageR = iR;
	imageL = iL;
	initialNCC = iNCC;	
	finalNCC = fNCC;
	glareNCC = gNCC;
	initialHomography = new double[9];
	finalHomography = new double[9];
	for (int i = 0; i < 9; i++)
	{
		initialHomography[i] = iH[i];
		finalHomography[i] = fH[i];
	}
}

dataCollection::dataCollection(char ** entry)
{
	version = atoi(entry[0]);
	tile = entry[1];
	smallImage = entry[2];
	imageR = entry[3];
	imageL = entry[4];
	initialNCC = stod(entry[5]);
	finalNCC = stod(entry[6]);
	glareNCC = stod(entry[7]);
	initialHomography = new double[9];
	finalHomography = new double[9];
	int count = 8;
	for (int i = 0; i < 9; i++)
	{
		initialHomography[i] = (double)atoi(entry[count]);
		finalHomography[i] = (double)atoi(entry[count+1]);
		count += 2;
	}

}

void dataCollection::writeEntry() //write entry to log file.
{
	ofstream stream;
	stream.open("tileData.txt", std::ofstream::out | std::ofstream::app);

	stream << version << " ";
	stream << tile << " ";
	stream << smallImage << " ";
	stream << imageR << " " << imageL << " ";
	stream << initialNCC << " " << finalNCC << " " << glareNCC << " ";
	for (int i = 0; i < 9; i++)
	{
		stream << initialHomography[i] << " ";
	}

	for (int i = 0; i < 9; i++)
	{
		stream << finalHomography[i] << " ";
	}

	stream << endl;

	stream.close();
}

void dataCollection::print() //print object to terminal or redirect to file
{
	cout << version << " ";
	cout << tile << " ";
	cout << smallImage << " ";
	cout << imageR << " " << imageL << " ";
	cout << initialNCC << " " << finalNCC << " " << glareNCC << " ";
	for (int i = 0; i < 9; i++)
	{
		cout << initialHomography[i] << " ";
	}

	for (int i = 0; i < 9; i++)
	{
		cout << finalHomography[i] << " ";
	}

	cout << endl;
}

vector<dataCollection> dataCollection::checkFile(string condition, int index) //static
{
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];
	vector<dataCollection> entries;

	int numLines = 0;
	ifstream in("tileData.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("tileData.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		line[sizeof(line)] = '\0';
		parser(parsed_line, line);
		if (parsed_line[index] == condition)
		{
			dataCollection entry(parsed_line);
			entries.push_back(entry);
			entry.print();
		}
		entry_count++;
	}

	stream.close();

	return entries;
}

vector<dataCollection> dataCollection::checkFile(string condition1, string condition2, int index)
{
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];
	vector<dataCollection> entries;

	int numLines = 0;
	ifstream in("tileData.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("tileData.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		line[sizeof(line)] = '\0';
		parser(parsed_line, line);
		if (parsed_line[index] == condition1 && parsed_line[index+1] == condition2)
		{
			dataCollection entry(parsed_line);
			entries.push_back(entry);
			entry.print();
		}
		entry_count++;
	}

	stream.close();

	return entries;
}

vector<dataCollection> dataCollection::getAll() //Static. Returns a vector of every entry
{
	vector<dataCollection> entries;
	int entry_count = 0;
	ifstream stream;
	string line;
	char ** parsed_line = new char * [1000];

	int numLines = 0;
	ifstream in("tileData.txt");
	std::string unused;
	while ( getline(in, unused) )
	   ++numLines;

	stream.open("tileData.txt", std::ofstream::in);

	while(getline(stream, line))
	{
		line[sizeof(line)] = '\0';
		parser(parsed_line, line);
		for (int count = 0; count < 26; count++)
		{
			cout << parsed_line[count] << " ";
		}
		cout << endl;
		entry_count++;
	}

	stream.close();	
	return entries;
}

vector<dataCollection> dataCollection::getEntriesByVersion(int v) //static
{
	return checkFile(to_string(v), 0);
}

vector<dataCollection> dataCollection::getEntriesByImage(string i) //Static. Returns entries associated with given image.
{
	return checkFile(i, 2);
}

vector<dataCollection> dataCollection::getEntriesByImagePair(string i1, string i2) //Static. Returns entries associated with given image pair.
{
	return checkFile(i1, i2, 2);
}

vector<dataCollection> dataCollection::getEntriesByTile(string t) //Static. Returns entries associated with given tile
{
	return checkFile(t, 0);
}

vector<dataCollection> dataCollection::getEntriesByNCCRange(float iNCC, float fNCC) //Static. Returns entries between given range of NCC values.
{
	return checkFile(to_string(iNCC), to_string(fNCC), 4);
}
