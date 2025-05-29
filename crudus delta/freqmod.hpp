#pragma once
#include <vector>
#include "wave.hpp"

namespace Synth
{
    using namespace std;
    template<typename T>
    wave<T> fm(wave<T> carrier, wave<T> modulator, double strength = 1)
    {
        wave<T> output(carrier.length);
        for (int i = 0; i < carrier.length; i++)
        {
            output[i] = carrier[i + (T)(((double)modulator[i])*strength)];
        }
        
    }
} // namespace Synth