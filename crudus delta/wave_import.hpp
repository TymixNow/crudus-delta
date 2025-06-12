#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "wave.hpp"
namespace WaveImport
{
	using namespace std;
	template<typename T>
	void get(ifstream &stream, Synth::wave<T>& wv)
	{
		vector<T> data;
		string line;
		getline(stream, line);
		stringstream reader(line);
		string segment;
		while (getline(reader, segment, ' '))
		{
			data.push_back(stoi(segment));
		}
		wv = Synth::wave<T>(data);
		return;
	}
}