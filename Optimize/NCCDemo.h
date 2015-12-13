#ifndef _NCC_
#define _NCC_

#include <iostream>
#include <vector>
#include <cmath>
#include "eriolHeader.h"
#include "homography.h"
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

//########################Color Functions#################################

double calculate_correlation(const vector<Color> & first_signal, const vector<Color> & second_signal)

{
	int size_of_signal = first_signal.size();
	//cerr << size_of_signal << endl;
	//Right now signals have to be the same size.
	double signal_correlationR = 0;
	double signal_correlationG = 0;
	double signal_correlationB = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		signal_correlationR += first_signal[count].r*second_signal[count].r;
		//cerr << first_signal[count] << endl;
		signal_correlationG += first_signal[count].g*second_signal[count].g;
		signal_correlationB += first_signal[count].b*second_signal[count].b;
	}
	//cerr << signal_correlationR << endl;
	return (signal_correlationR+signal_correlationG+signal_correlationB)/3;

}

double calculate_normalized_correlation(const vector<Color> & first_signal, const vector<Color> & second_signal)
{
	int size_of_signal = first_signal.size();
	double correlation = calculate_correlation(first_signal, second_signal);
	//cerr << correlation << endl;
	double sum_first_signalR = 0;
	double sum_second_signalR = 0;
	double sum_first_signalG = 0;
	double sum_second_signalG = 0;
	double sum_first_signalB = 0;
	double sum_second_signalB = 0;
	int correlation_scalar = 0;

	for (int count = 0; count < size_of_signal; count++)
	{
		sum_first_signalR += first_signal[count].r * first_signal[count].r;
		sum_second_signalR += second_signal[count].r * second_signal[count].r;
		sum_first_signalG += first_signal[count].g * first_signal[count].g;
		sum_second_signalG += second_signal[count].g * second_signal[count].g;
		sum_first_signalB += first_signal[count].b * first_signal[count].b;
		sum_second_signalB += second_signal[count].b * second_signal[count].b;
	}
	double total_sum1 = (sum_first_signalR + sum_first_signalG + sum_first_signalB) / 3; 
	double total_sum2 = (sum_second_signalR + sum_second_signalG + sum_second_signalB) / 3; 
	correlation_scalar = sqrt(total_sum1*total_sum2);
	if(correlation_scalar == 0){
		return -1;
	}else{
		return (double)correlation/correlation_scalar;
	}
}

#endif
