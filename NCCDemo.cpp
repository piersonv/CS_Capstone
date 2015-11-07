#include <iostream>
#include <vector>
#include <cmath>
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

vector<int> calculate_correlation(const int & size_of_signal, const vector<Color> & first_signal, const vector<Color> & second_signal)
{
	//Right now signals have to be the same size.
  vector<int> signal_correlation [3];

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
		sum_first_signal += first_signal[count] * first_signal[count];
		sum_second_signal += second_signal[count] * second_signal[count];
	}

	correlation_scalar_red = sqrt(sum_first_signal_red*sum_second_signal_red);
	correlation_scalar_blue = sqrt(sum_first_signal_blue*sum_second_signal_blue);
	correlation_scalar_blue = sqrt(sum_first_signal_green*sum_second_signal_blue);
	

	return (float)(correlation.red/correlation_scalar_red;

}

int main()
{
	vector<int> first_signal;
	vector<int> second_signal;
	float normalized_correlation;

	cout << "Please Enter first signal array: ";
	allocate_array(first_signal);

 	cout << "Please Enter second signal array: ";

	allocate_array(second_signal);

	normalized_correlation = calculate_normalized_correlation(first_signal, second_signal);

	cout << "Signal correlation: " << normalized_correlation << endl;

	return 0;

}
