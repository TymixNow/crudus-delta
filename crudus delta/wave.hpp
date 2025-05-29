#pragma once
#include "math_fix.hpp"
namespace Synth
{
    #include <vector>
    using namespace std;
    template <typename T>
    class wave
    {
    private:
        std::vector<T> data;
    public:
        wave  (std::vector<T> d) {data = d;}
        wave (int x) {data = std::vector(x)}
        ~wave () {}
        T operator [](T i) 
        {
            return data[mod(i, data.size())];
        }
        int length
        {
            return data.size();
        }
    };
} // namespace Synth