#include <iostream>
#include <vector>
#include <cmath>
#include "eriolHeader.h"
using namespace std;

void allocate_array(vector<int> & array_signal)
{
	char value;

	while (true)
	{
		value = cin.get();

		if (value == '\n')
			break;

		array_signal.push_back((int)value-48);
	}
}

void print_array(const vector<int> & signal)
{
	int count = 0;
	while(signal[count] !='\0')
	{
		cout << signal[count] << endl;
		count++;
	}

	cout << endl;
}

int * calculate_correlation(const int & size_of_signal, const vector<Color> & first_signal, const vector<Color> & second_signal)
{
	//Right now signals have to be the same size.
  int * signal_correlation = (int *) malloc(3*sizeof(int));

	for (int count = 0; count < size_of_signal; count++)
	{
	  signal_correlation[0] += first_signal[count].red*second_signal[count].red; //red channel
	  signal_correlation[1] += first_signal[count].blue*second_signal[count].blue; //blue channel
	  signal_correlation[2] += first_signal[count].green*second_signal[count].green; //green channel
	}

	return signal_correlation;

}

float calculate_normalized_correlation(const vector<Color> & first_signal, const vector<Color> & second_signal)
{
	int size_of_signal = first_signal.size();
	vector<int> correlation = calculate_correlation(size_of_signal, first_signal, second_signal);
	int sum_first_signal_red = 0;
	int sum_first_signal_blue = 0;
	int sum_first_signal_green = 0;
	int sum_second_signal_red = 0;
	int sum_second_signal_blue = 0;
	int sum_second_signal_green = 0;
	int correlation_scalar_red = 0;
	int correlation_scalar_blue = 0;
	int correlation_scalar_green = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		sum_first_signal_red += first_signal[count].red * first_signal[count].red;
		sum_second_signal_red += second_signal[count].red * second_signal[count].red;

		sum_first_signal_blue += first_signal[count].blue * first_signal[count].blue;
		sum_second_signal_blue += second_signal[count].blue * second_signal[count].blue;

		sum_first_signal_green += first_signal[count].green * first_signal[count].green;
		sum_second_signal_green += second_signal[count].green * second_signal[count].green;
	}

	correlation_scalar_red = sqrt(sum_first_signal_red*sum_second_signal_red);
	correlation_scalar_blue = sqrt(sum_first_signal_blue*sum_second_signal_blue);
	correlation_scalar_blue = sqrt(sum_first_signal_green*sum_second_signal_blue);
	

	return (float)((correlation[0]/correlation_scalar_red) 
		* (correlation[1]/correlation_scalar_blue) * (correlation[2]/correlation_scalar_green))/3;

}

int main()
{
	vector<Color> first_signal;
	vector<Color> second_signal;
	float normalized_correlation;

	normalized_correlation = calculate_normalized_correlation(first_signal, second_signal);

	cout << "Signal correlation: " << normalized_correlation << endl;

	return 0;

}
