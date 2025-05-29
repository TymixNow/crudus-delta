#pragma once
#include <cmath>
namespace FreqGen
{
    class FreqGen
    {
    public:
        static int gen(int i)
        {
            int l;
            float f;
            f = pow(2.0, ((i - 69.0) / 12.0)) * 440.0;
            l = 44100.0 / f;
            return l;
        }
    };
} // namespace FreqGen