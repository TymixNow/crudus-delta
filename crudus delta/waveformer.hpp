#pragma once
#include "wavstream.hpp"
#include "freq_gen.hpp"
#include "wave.hpp"

namespace Waveformer
{

    using namespace std;
    using namespace WavWrapper;
    class Waveform
    {
    public:
        Waveform(Synth::wave<sample> in, double time)
        {
            data = in;
            t = time;
        }
        void sync(double& time)
        {
            time = t;
        }
        vector<sample> Render(int note, double length)
        {
            // soundframes per cycle: FreqGen::FreqGen::gen(note);
            vector<sample> output;
            double dt;
            dt = ((double)data.size()) / FreqGen::FreqGen::gen(note);
            for (int i = 0; i < length; i++)
            {
                output.push_back(data[(int)t]);
                t += dt;
                while (t > data.size()) t -= data.size();
            }
            return output;
        }
        ~Waveform()
        {
        }

    private:
        double t;
        Synth::wave<sample> data;
    };
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
