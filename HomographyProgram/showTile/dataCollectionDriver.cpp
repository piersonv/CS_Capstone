#include "dataCollection.h"
using namespace std;

int main()
{
	//dataCollection::getAll();
	double m1 [9], m2 [9]; 
	for (int i = 0; i < 9; i++)
	{
		m1[i] = m2[i] = 1;
	}
	dataCollection dC("hi", "he", "hr", "hw", 3, 4, m1, m2);
	dC.print();
	dC.writeEntry();
	return 0;
}