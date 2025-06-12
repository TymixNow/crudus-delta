#pragma once
#include <vector>
#include "math_fix.hpp"
#include "wave.hpp"

namespace Synth
{
    using namespace std;
    template<typename T>
    wave<T> fm(wave<T> carrier, wave<T> modulator, double strength = 1)
    {
        int size = Mathfix::lcm(carrier.size(), modulator.size());
        wave<T> output(size);
        for (int i = 0; i < size; i++)
        {
            output[i] = carrier[i + (T)(((double)modulator[i])*strength)];
        }
        return output;
    }
} // namespace Synth