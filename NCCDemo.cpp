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

int calculate_correlation(const int & size_of_signal, const vector<int> & first_signal, const vector<int> & second_signal)
{
	//Right now signals have to be the same size.
	int signal_correlation = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		signal_correlation += first_signal[count]*second_signal[count];
	}

	return signal_correlation;

}

float calculate_normalized_correlation(const vector<int> & first_signal, const vector<int> & second_signal)
{
	int size_of_signal = first_signal.size();
	int correlation = calculate_correlation(size_of_signal, first_signal, second_signal);
	int sum_first_signal = 0;
	int sum_second_signal = 0;
	int correlation_scalar = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		sum_first_signal += first_signal[count] * first_signal[count];
		sum_second_signal += second_signal[count] * second_signal[count];
	}

	correlation_scalar = sqrt(sum_first_signal*sum_second_signal);

	return (float)correlation/correlation_scalar;

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