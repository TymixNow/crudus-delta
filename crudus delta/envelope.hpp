#pragma once
#include <vector>
#include "waveformer.hpp"
class envelope
{
public:
	envelope(double d)
	{
		damp = d;
		wave = {};
	}

	void operator<<(std::vector<sample> v)
	{
		wave.insert(wave.end(), v.begin(), v.end());
	}

	void Render()
	{
		double volume = 1;
		for (auto& s : wave)
		{
			s *= volume;
			volume *= damp;
			s ^= 0x80;
		}
	}

	void operator>>(std::vector<sample>& v)
	{
		v = wave;
	}

	~envelope()
	{
	}

private:
	std::vector<sample> wave;
	double damp;
};
