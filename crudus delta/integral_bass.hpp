#pragma once
#include "wave.hpp"
#include <algorithm>
namespace Synth
{
	template <typename T>
	wave<T> prefix_sum(wave<T> primary)
	{
		wave<T> output(primary.size());
		double sum = 0;
		for (int i = 0; i < primary.size(); i++)
		{
			output[i] = (T)(sum/primary.size());
			sum += primary[i];
		}
		return normalize(output);
	}
	template <typename T>
	wave<T> odd_bass(wave<T> primary)
	{
		wave<T> output(primary.size() * 2);
		for (int i = 0; i < primary.size(); i++)
		{
			output[i] = primary[i];
			output[i + primary.size()] = primary[primary.size() - i];
		}
		return normalize(output);
	}
	template <typename T>
	wave<T> normalize(wave<T> primary)
	{
		wave<T> output(primary.size());
		vector<T> vec = primary.get();
		T max = std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
		T min = std::distance(vec.begin(), std::min_element(vec.begin(), vec.end()));
		for (int i = 0; i < primary.size(); i++)
		{
			output[i] = ((double)primary[i] - min)/((double)max - min)*(255) - 127;
		}
		return output;
	}
}