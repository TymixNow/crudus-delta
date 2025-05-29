#pragma once
#include "wavstream.hpp"
#include "freq_gen.hpp"

namespace Waveformer
{

    using namespace std;
    using namespace WavWrapper;

    vector<sample> Repeat(vector<sample> waveform, unsigned int note, unsigned int length)
    {
        int period = FreqGen::FreqGen::gen(note)/waveform.size();
        vector<sample> wave;
        for (int i = 0; i < waveform.size(); i++)
        {
            vector<sample> reps(period, waveform[i]);        
            wave += reps;
        }
        vector<sample> stream;
        for (int i = 0; i < length; i++)
        {
            stream += wave;
        }
        return stream;
    }
} // namespace Waveformer
