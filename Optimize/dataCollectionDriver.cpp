#include "dataCollection.h"
using namespace std;

int main()
{
	double m1 [9], m2 [9]; 
	for (int i = 0; i < 9; i++)
	{
		m1[i] = m2[i] = 1;
	}
	dataCollection dC(1, "hi", "he", "hr", "hw", 0.95, 4, 5, m1, m2);
	dC.writeEntry();
	dC.print();
	vector<dataCollection> entries;
	entries = dataCollection::getEntriesByImagePair("he", "r");
	return 0;
}