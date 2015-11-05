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

float calculate_correlation(const int & size_of_signal, const vector<float> & first_signal, const vector<float> & second_signal)
{
	//Right now signals have to be the same size.
	float signal_correlation = 0;

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

double calculate_normalized_correlation(const vector<double> & first_signal, const vector<double> & second_signal)
{
	int size_of_signal = first_signal.size();
	double correlation = calculate_correlation(size_of_signal, first_signal, second_signal);
	double sum_first_signal = 0;
	double sum_second_signal = 0;
	int correlation_scalar = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		sum_first_signal += first_signal[count] * first_signal[count];
		sum_second_signal += second_signal[count] * second_signal[count];
	}

	correlation_scalar = sqrt(sum_first_signal*sum_second_signal);

	return (double)correlation/correlation_scalar;

}
